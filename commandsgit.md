# Alguns comandos básicos do Git

* inicializa um repositorio &nbsp;
git init

* cria uma nova branch
&nbsp; 
git branch new_branch

* muda para um branch
&nbsp;
git checkout new_branch

* configurar git
&nbsp;
git config --global user.name "NOME"
&nbsp;
git config --global user.email "meuemail@github.com"

* adiciona os arquivos no repositório git
&nbsp;
git add --all

* commit no repositorio
&nbsp;
git commit -m "Adicionado arquivo readme.md"

* cria uma ligação remota com o github
&nbsp;
git remote add origin https://github.com/USUARIO/meu-primeiro-repositorio.git

* enviar arquivos pro github
&nbsp;
git push -u origin master

* retornar arquivos do github
&nbsp;
git pull
