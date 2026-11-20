# C programming problem sets

Problem sets from the Programming course at TU Košice.

| Folder | Program |
|---|---|
| `ps1` | Command-line cipher tool: Playfair, Vigenère, bit-level and BMP-based encryption/decryption |
| `ps2` | Console tile-merging game `k` on a 4×4 board (ncurses UI) with a hall of fame |
| `ps4` | BMP image editor: flip, rotate, scale, crop, channel extraction |
| `ps5` | Text adventure engine: rooms, items, containers, backpack, command parser, scenario loaded from `scenario.txt` |

## Build

Each folder has a `Makefile`:

```bash
cd ps4
make
```

`ps1` and others run `uncrustify` and `cppcheck` as part of the Makefile, so install them first (`sudo apt install uncrustify cppcheck`); `ps2` needs ncurses (`libncurses-dev`).

## License

MIT, see [LICENSE](LICENSE).
