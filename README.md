# Cannon
Simulator supporting a user interface for Cannon, an abstract strategy board game.

## Details
The GameServer repo contains the server and client codes to run the game. These have been developed by the TAs of COL333.

The Bot directory has been built by [Sameer Pande](https://github.com/sameerpande12) and [Pratheek Rebello](https://github.com/PratheekRebello).
The rules of the game can be found in the GameServer's [readme](GameServer/README.md)

The AI bot is a adversial-search based bot. It uses techniques such as alpha-beta pruning, memoization and a strategic evaluation function to play the game. The bot can search upton depths 5 within 1-2 seconds and depth 6 in 5-6 seconds. The bot automatically decides till what depths to search based on the time left and current stage of the game.

## Running The Game

After the setup steps have been followed as specified, here are the steps to run the game.


1. Run the server
    * `cd GameServer`
    *  `python server.py <port> -n <N> -m <M> -NC 2 -TL <TimeLimit> -LOG server.log `
    *  (N,M) values that are supported by server are (8,8), (10,8), (10,10)
2. Run the player
    * `cd GameServer`
    *  `python client.py 0.0.0.0 <port> <playing_bot> -mode GUI`
    * `<playing_bot>` can be the bot placed in Bot repostiory, or a random bot.
    *  `python client.py 0.0.0.0 <port> ../Bot/run.sh -mode GUI` runs our bot
    *  `python client.py 0.0.0.0 <port> RandomPlayer.py -mode GUI` runs a random player

3. In the convention of the game, player 1 is black and black plays first.
4. The Learning Code of the repository was used to learn parameters.

