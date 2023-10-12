## simple zip compressor/extractor in c for self use

###

<div align="center">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-line.svg" height="200" alt="c logo"  />
</div>

###

## project structure:

```c
.
├── README.md
├── .gitignore
├── LICENSE
└── zipman.c
```

## installation

```shell
git clone github.com/kenjitheman/zipman.c
```

## usage

- 1

```sh
gcc -o zipman zipman.c -lz
```

```sh
[?] usage: ./zipman [compress|extract] [input_file] [output_file]
```

## example

```sh
➜ ./zipman compress ./man.txt ./man.zip
[SUCCESS] compression completed successfully
```

## contributing

- pull requests are welcome, for major changes, please open an issue first to
  discuss what you would like to change

## license

- [MIT](https://choosealicense.com/licenses/mit/
