"""Black-box regression tests against the real interactive executable (stdlib only)."""
import argparse
from pathlib import Path
import subprocess
import tempfile
import unittest


parser = argparse.ArgumentParser()
parser.add_argument("--exe", required=True)
args, remaining = parser.parse_known_args()
EXE = str(Path(args.exe).resolve())


class SaveLoadTests(unittest.TestCase):
    def setUp(self):
        test_root = Path(__file__).resolve().parents[1] / "build" / "test-runs"
        test_root.mkdir(parents=True, exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=test_root)
        self.addCleanup(self.temp.cleanup)
        self.cwd = Path(self.temp.name)

    def run_game(self, *commands):
        result = subprocess.run(
            [EXE], input="\n".join((*commands, "quit")) + "\n",
            text=True, capture_output=True, cwd=self.cwd, timeout=10,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        return result.stdout

    def read(self, name):
        return (self.cwd / name).read_text()

    def write(self, name, text):
        (self.cwd / name).write_bytes(text.encode("utf-8"))

    def test_original_inventory_loss(self):
        # ID 0 in the last room used to make the EOF iteration clear inventory.
        output = self.run_game(
            "get golden_apple", "w", "get melon_knife", "drop golden_apple",
            "save before", "load before", "inventory", "save after",
        )
        self.assertEqual(self.read("before"), self.read("after"))
        self.assertIn("Melon Knife", output.split("Object(s) in the inventory:")[-1])

    def test_original_room_loss(self):
        # ID 1 in the last room used to clear room 1, losing the apple there.
        self.run_game("n", "get honey_cake", "s", "w", "drop honey_cake",
                      "save before", "load before", "save after")
        self.assertEqual(self.read("before"), self.read("after"))

    def test_round_trip_after_moving_eating_and_repeated_loading(self):
        self.run_game("get golden_apple", "eat golden_apple", "e", "get ak47",
                      "get brownie", "eat brownie", "w", "s", "drop ak47",
                      "save before", "get sugar_rush", "n", "w", "get melon_knife",
                      "load before", "load before", "save after")
        self.assertEqual(self.read("before"), self.read("after"))
        remaining_objects = [int(value) for line in self.read("after").splitlines()[2:]
                             for value in line.split()[1:]]
        self.assertNotIn(0, remaining_objects)
        self.assertNotIn(2, remaining_objects)

    def test_cross_process(self):
        self.run_game("get golden_apple", "e", "get ak47", "save before")
        self.run_game("load before", "save after")
        self.assertEqual(self.read("before"), self.read("after"))

    def test_valid_whitespace_and_row_order(self):
        self.run_game("get golden_apple", "w", "get melon_knife", "save baseline")
        original = self.read("baseline")
        lines = original.splitlines()
        variants = {
            "no_trailing_spaces": "\n".join(line.rstrip() for line in lines) + "\n",
            "no_final_newline": "\n".join(line.rstrip() for line in lines),
            "blank_lines": "\n".join(lines[:2] + [""] + lines[2:] + ["", "", "   "]),
            "tabs": "\n".join([lines[0]] + [line.replace(" ", "\t") for line in lines[1:]]),
            "reordered": "\n".join(lines[:2] + list(reversed(lines[2:]))) + "\n",
            "crlf": original.replace("\n", "\r\n"),
        }
        for name, text in variants.items():
            with self.subTest(name=name):
                self.write("candidate", text)
                output = self.run_game("load candidate", "save after")
                self.assertIn("Loaded successfully.", output)
                self.assertEqual(original, self.read("after"))

    def test_invalid_files_leave_entire_state_unchanged(self):
        self.run_game("save initial")
        initial = self.read("initial")
        lines = initial.splitlines()
        variants = {
            "empty": "",
            "wrong_version": initial.replace("1.3.0", "9.9.9"),
            "header_only": lines[0] + "\n",
            "no_locations": "\n".join(lines[:2]),
            "missing_inventory": "\n".join(lines[:2] + lines[3:]),
            "missing_room": "\n".join(lines[:-1]),
            "unknown_room": initial.replace("100 1", "100 99"),
            "unknown_location": initial + "99 0\n",
            "unknown_object": initial.replace("5 6", "5 99"),
            "duplicate_location": initial + "1\n",
            "duplicate_object": initial.replace("0 \n", "0 0\n"),
            "duplicate_within_room": initial.replace("5 6", "5 6 6"),
            "bad_object_token": initial.replace("5 6", "5 nope"),
            "trailing_garbage": initial + "junk\n",
            "bad_strength": initial.replace("100 1", "nope 1"),
            "negative_strength": initial.replace("100 1", "-1 1"),
            "excess_strength": initial.replace("100 1", "101 1"),
            "zero_strength": initial.replace("100 1", "0 1"),
            "extra_header_field": initial.replace("100 1", "100 1 2"),
            "negative_id": initial.replace("5 6", "5 -1"),
            "overflow_id": initial.replace("5 6", "5 65536"),
            "huge_id": initial.replace("5 6", "5 " + "9" * 100),
        }
        for name, text in variants.items():
            with self.subTest(name=name):
                self.write("candidate", text)
                output = self.run_game("get golden_apple", "n", "get honey_cake",
                                       "save before", "load candidate", "save after")
                self.assertIn("The save file is not a valid file.", output)
                self.assertNotIn("Loaded successfully.", output)
                self.assertEqual(self.read("before"), self.read("after"))

    def test_missing_file_preserves_state(self):
        output = self.run_game("get golden_apple", "save before", "load missing", "save after")
        self.assertIn("The save file doesn't exist.", output)
        self.assertEqual(self.read("before"), self.read("after"))

    def test_strength_and_room_are_restored(self):
        self.run_game("get golden_apple", "n", "save baseline")
        self.write("candidate", self.read("baseline").replace("100 2", "70 2"))
        output = self.run_game("load candidate", "eat golden_apple", "save eaten",
                               "s", "load candidate", "save restored")
        self.assertIn("Your strength is 80", output)
        self.assertEqual(self.read("eaten").splitlines()[1], "80 2")
        self.assertEqual(self.read("candidate"), self.read("restored"))

    def test_save_open_failure_is_reported(self):
        output = self.run_game("save missing-directory/slot")
        self.assertIn("Could not save the game.", output)
        self.assertNotIn("Saved successfully.", output)

    def test_eof_exits_without_quit(self):
        result = subprocess.run([EXE], input="inventory\n", text=True,
                                capture_output=True, cwd=self.cwd, timeout=3)
        self.assertEqual(result.returncode, 0)


if __name__ == "__main__":
    unittest.main(argv=[__file__, *remaining], verbosity=2)
