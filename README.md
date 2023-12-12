## Simple zip compressor/extractor in c for self use

###

<div align="center">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-line.svg" height="200" alt="c logo"  />
</div>

###

## Project structure:

```c
zipman.c
│
├── README.md
├── .gitignore
├── LICENSE
└── zipman.c
```

## Installation

```sh
git clone https://github.com/kenjitheman/zipman.c
```

## Usage

```sh
gcc -o zipman zipman.c -lz
```

- [?] usage: ./zipman [compress|extract] [input_file] [output_file]

## Example

```sh
➜ ./zipman compress ./man.txt ./man.zip
[SUCCESS] compression completed successfully
```

## Contributing

- Pull requests are welcome, for major changes, please open an issue first to
  discuss what you would like to change.

## License

- [MIT](https://choosealicense.com/licenses/mit/)
