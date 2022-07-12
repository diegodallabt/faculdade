class Player{
    constructor(name, mana, hp, skills){
        this.name = name;
        this.mana = mana;
        this.hp = hp;
        this.skills = skills;
    }

    batalhar(){
        console.log("A batalha começou!")
    }
}

const player1 = new Player('dks', 100, 100, [-30, 10, -15]);
const player2 = new Player('BOT', 100, 100, [-5, 25, -20]);

console.log('O ' + player1.name + ' está enfrentando o ' + player2.name + '.');

player1.batalhar()
