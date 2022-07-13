function Player(name, hp, level, xp){
    this.name = name;
    this.hp = hp;
    this.level = level;
    this.xp = xp;
    this.spells = [-30, 10, -15];
}

const player1 = new Player('dk$', 100, 1, 0);
const player2 = new Player('godin', 100, 1, 0);

let player_1_skill;
    let player_2_skill;
    for (let round = 0; round < 10; round++) {
        console.log('Round ' + round + ':');
        player_1_skill = Math.floor(Math.random() * 3);
        player_2_skill = Math.floor(Math.random() * 3);
        console.log('Player 1: ' + player1.spells[player_1_skill]);
        console.log('Player 2: ' + player2.spells[player_2_skill]);
        switch (player_1_skill) {
            case 0:
                player2.hp += player1.spells[player_1_skill];
                console.log('HP2: ' + player2.hp);
                if(player2.hp <= 0){
                    console.log('Player 2 foi eliminado.');
                }
                break;
            case 1:
                if(player1.hp < 100){
                    player1.hp += player1.spells[player_1_skill];
                }
                    console.log('HP1: ' + player2.hp);
                break;
            case 2:
                player2.hp += player1.spells[player_1_skill];
                console.log('HP2: ' + player2.hp);
                if(player2.hp <= 0){
                    console.log('Player 2 foi eliminado.');
                }
                break;
            default:
                break;
        }

        switch (player_2_skill) {
            case 0:
                player1.hp += player2.spells[player_2_skill];
                console.log('HP1: ' + player1.hp);
                if(player1.hp <= 0){
                    console.log('Player 1 foi eliminado.');
                }
                break;
            case 1:
                if(player2.hp < 100){
                    player2.hp += player2.spells[player_2_skill];
                }
                    console.log('HP2: ' + player1.hp);
                break;
            case 2:
                player1.hp += player2.spells[player_2_skill];
                console.log('HP1: ' + player1.hp);
                if(player1.hp <= 0){
                    console.log('Player 1 foi eliminado.');
                }
                break;
            default:
                break;
        }
        console.log('\n');
    }