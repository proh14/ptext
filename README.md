<h1 align="center">
  <br>
  <img src="./images/logo.png" alt="ptext logo" width="200">
  <br>
  ptext
  <br>
</h1>
<h4 align="center">A nano like text editor built with pure C </h4>

ptext is a text editor based on kilo. <br> 
My goal in making this was to learn more about managing tui interfaces not using ncurses.</br> 

### Preview🙈:
https://github.com/proh14/ptext/assets/95998030/08ee068b-fe9b-4d3f-b3e4-49d3a66855d5

### How to install⬇️:
1. Clone this repo.
```shell
git clone https://github.com/proh14/ptext.git
cd ptext
```
2. Run `make` inside this repo's main directory.
```shell
make
```

3. go to build directory
```shell
cd build
```
4. run ptext!
```shell
./ptext <filename>
```

OR to install you may run the command
```shell
make install
```
then restart your shell and run
```shell
ptext <file name>
``` 
to start the editor.

### Documentation📖:
If you installed ptext on a *nix system you can simply run 
``` shell
man ptext
```
to see the maual page of ptext.
Otherwise take a look at [./docs](./docs] directory to see the manual in diffrent formats!
The markdown version of the [manual](./docs/manual.md)

### Goals🥅:
1. Add a plugin system (for example .ptextrc).
2. Add windows.
3. Add utf support.
4. Read the terminfo database.

### Work in progress⚒️:
This is still a work in progress project!

### Contributions💖:
I appreciate contributions but first of all, you must read the <br>
Read the [CONTRIBUTING.md](CONTRIBUTING.md) Then you can send your pr!

### Thank you Stargazers⭐:
[![Stargazers repo roster for @proh14/ptext](http://reporoster.com/stars/proh14/ptext)](https://github.com/proh14/ptext/stargazers)

### Thank you Forkers🍴:
[![Forkers repo roster for @proh14/ptext](http://reporoster.com/forks/proh14/ptext)](https://github.com/proh14/ptext/network/members)

### A huge thanks to🙏:
1. [viewsourcecode](https://viewsourcecode.org)
2. [kilo](https://github.com/antirez/kilo)
