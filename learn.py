 
from __future__ import print_function

import os
import sys
import time
import copy
import json
import math
import numpy as np
import sys
import subprocess




def getScore(server_log):
    f = open(server_log,'r')
    lines = [line for line in f.readlines()]


    result = lines[-1]
    result = result.split('}')
    result = result[0]+'}'
    result = json.loads(result)
    result = result['meta'].encode('ascii')

    result = result.split(':')
    result = [s.strip() for s in result]

    score_p1 = float(result[1])
    score_p2 = float(result[3])

    if result[0] == 'Player 2 SCORE':
        score_p2,score_p1 = score_p1,score_p2
    f.close()
    return score_p1,score_p2


def execute(command):
    return subprocess.Popen([command],shell=True,executable='/bin/bash')

def getWeights(weightFile):
    weights = subprocess.check_output('tail -1 {}'.format(weightFile),shell=True,executable='/bin/bash')
    weights = weights.strip()
    weights = weights.split()
    weights = [float(weight) for weight in weights]
    return weights

def generateParamFile(paramFile,lr,weights):
    file = open(paramFile,'w+')
    line = str(lr)
    for weight in weights:
        line = line + " " + str(weight)
    file.write(line)
    file.close()

gameid = 0
baseWeights = [1,10,10,1000]
generateParamFile('param1.txt',0.1,[1,6,11,1000])
generateParamFile('param2.txt',0,baseWeights)## We intend to keep run2 stable always


# run1.sh plays black
for gameid in range(1,101):
    command='mkdir -p Logs/Game_{}/Run1'.format(gameid)
    execute(command)
    command='mkdir -p Logs/Game_{}/Run2'.format(gameid)
    execute(command)

    print("Run1")
    command='python server.py 10000 -n 8 -m 8 -NC 2 -TL 150 -LOG server{}_1.log'.format(gameid)
    print(command)
    server_process = execute(command)

    command=' python client.py 0.0.0.0 10000 run1.sh '
    print(command)
    execute(command)


    command=' python client.py 0.0.0.0 10000 run2.sh'
    print(command)
    execute(command)
    

    server_process.wait()
    g1_score_1,g1_score_2 = getScore('server{}_1.log'.format(gameid))


    
    command='mv weights1.txt weights2.txt server{}_1.log Logs/Game_{}/Run1'.format(gameid,gameid)
    execute(command)


    # run1.sh plays white
    print("Run2")

    command='python server.py 10000 -n 8 -m 8 -NC 2 -TL 150 -LOG server{}_2.log'.format(gameid)
    print(command)
    server_process = execute(command)
    
    command=' python client.py 0.0.0.0 10000 run2.sh '
    print(command)
    execute(command)
    
    command=' python client.py 0.0.0.0 10000 run1.sh'
    print(command)
    execute(command)
    
    server_process.wait()
    g2_score_2,g2_score_1 = getScore('server{}_2.log'.format(gameid))

    command='mv weights1.txt weights2.txt server{}_2.log Logs/Game_{}/Run2'.format(gameid,gameid)
    execute(command)

    execute('mv param1.txt param2.txt Logs/Game_{}'.format(gameid))

    score_1 = g1_score_1 + g2_score_1
    score_2 = g1_score_2 + g2_score_2


    """
    If the player with dynamic weights loses then ->
        Start dynamic player with its last weights (the ones at end of the prev game)
        Start static player with its last weights

    If the player with dynamic weights wins then ->
        Dynamic Player starts from the base
        Static player uses the weights of previously winner dynamic winner(end weights)
    """
    if score_1 > score_2:
        execute(' echo {} >> Logs/result.txt'.format("{}:Dynamic, Score {} {} {} {}".format(gameid,g1_score_1,g1_score_2,g2_score_1,g2_score_2)))

        if g1_score_1 > g2_score_1:
            weightFile = 'Logs/Game_{}/Run1/weights1.txt'.format(gameid)
        else:
            weightFile = 'Logs/Game_{}/Run2/weights1.txt'.format(gameid)
        
        weights = getWeights(weightFile)
        generateParamFile('param2.txt',0,weights)# use winning weights for static
        generateParamFile('param1.txt',0.1,baseWeights)#start dynamic from previous base weights
        baseWeights = weights

    else:
        execute(' echo {} >> Logs/result.txt'.format("{}:Static, Score {} {} {} {}".format(gameid,g1_score_1,g1_score_2,g2_score_1,g2_score_2)))
        if g1_score_1 > g2_score_1:
            weightFile = 'Logs/Game_{}/Run1/weights1.txt'.format(gameid)
        else:
            weightFile = 'Logs/Game_{}/Run2/weights1.txt'.format(gameid)
        weights = getWeights(weightFile)
        generateParamFile('param1.txt',0.1,weights)
        generateParamFile('param2.txt',0,baseWeights)
