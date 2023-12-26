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
sudo make install
```
then restart your shell and run
```shell
ptext <file name>
``` 
to start the editor.

### A little bit of documentation📖:
- To exit you can press CTRL + q
- To save you can press CTRL + s
- To search you can press CTRL + f
- To goto the start of the line you can press CTRL + a
- To goto the end of the line you can press CTRL + e
- To goto the start of the file you can press CTRL + u
- To goto the end of the file you can press CTRL + d

### Goals🥅:
1. Fix syntax highlighting.
2. Add a plugin system.
3. Add windows.
4. Add utf support.
5. Read the terminfo database.

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
