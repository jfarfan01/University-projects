import grpc
import time
import sys
import math
from random import randint

sys.path.append('./netcode')

from netcode.tko_pb2 import *
from netcode.netcode_pb2 import *
from netcode.netcode_pb2_grpc import GameComStub

player_number = 0
ops_number = 0

# settings
userToken = '8950ba9c92977ac47c921d9f3b09f4da02eb102dcb5e7545d408d71cad7a6631'  # TODO: Insert your user token here
gameserverAddress = 'gameserver.ist.tugraz.at:80'

# Vars
matchToken = ""

# Setup comms
channel = grpc.insecure_channel(gameserverAddress)
stub = GameComStub(channel)

class Move:
    def __init__(self, stone, x, y, value):
        self.stone = stone
        self.x = x
        self.y = y
        self.value = value

        

# Query your userToken to be able to start a new match
def getUserToken(matr_number, secret):
    auth = AuthPacket()
    auth.matr_number = matr_number
    auth.secret = secret
    response = stub.GetUserToken(auth)
    return response.user_token


# Request a new match for Tak
def newMatch(length):
    params = GameParameter()
    request = MatchRequest(user_token=userToken,
                           game_token='tko',
                           timeout_suggestion_seconds=30, # 3600
                           tko_game_parameters=params)
    response = stub.NewMatch(request)

    print("New Match:", response.match_token)
    print("First player?", response.beginning_player)

    
    global player_number
    player_number = 2
    global ops_number
    ops_number = 1

    if response.beginning_player:
        player_number = 1
        ops_number = 2
        

    global matchToken
    matchToken = response.match_token

    


# Helper to create the MatchIDPacket
def createMatchId():
    return MatchIDPacket(user_token=userToken,
                         match_token=matchToken)


# Let's get started
def queryGameState():
    response = stub.GetGameState(createMatchId())
    return response.tko_game_state, response.game_status


# Sleepy time
def waitMatchStarted():
    while queryGameState()[1] == GameStatus.MATCH_NOT_STARTED:
        time.sleep(1)


# Identify yourself!
def queryOpponentInfo():
    response = stub.GetOpponentInfo(createMatchId())
    print("Opponent:", response.user_pseudonym, "(" + str(response.elo.user_elo) + "),",
          "from group:", response.group_pseudonym, "(" + str(response.elo.group_elo) + ")")
    pass


# What did we agree on? 2 Timeout, keine mehr?
def queryTimeout():
    response = stub.GetTimeout(createMatchId())
    print("Negotiated timeout:", response.timeout_seconds, "sec")
    pass


# This function prints the board by using the board data, printing it as a matrix.
def showGameState():
    state, status = queryGameState()
    print("Printing the board:")
    width = 5
    height = 5
    print("Board size:", width, "x", height)   
    byte_board = state.board
    for y in range(height): 
        row = []
        for x in range(width):
            row.append(byte_board[x + y * width])
        print(row)
                                        #weird formatting but kinda works I think
    return state, status


    


# Submit a turn and evaluate the turn status info. Also returns the Tak board state.
def submitTurn(turn):
    request = TurnRequest(match_id=createMatchId(),
                          tko_game_turn=turn)
    response = stub.SubmitTurn(request)
    if response.turn_status == TurnStatus.INVALID_TURN:
        print("Error: Invalid turn submitted.")
        exit(1)
    if response.turn_status == TurnStatus.MATCH_OVER:
        print("Match is over.")
        exit(0)
    if response.turn_status == TurnStatus.NOT_YOUR_TURN:
        print("This isn't the time to use that!")
    return response.tko_game_state


# Helper
def isMatchOver(game_status):
    return game_status in [GameStatus.MATCH_WON,
                           GameStatus.MATCH_LOST,
                           GameStatus.DRAW,
                           GameStatus.MATCH_ABORTED]

def printMatchOverMessage(game_status):
    if game_status == GameStatus.MATCH_WON:
        print("You won!")
    elif game_status == GameStatus.MATCH_LOST:
        print("You lost!")
    elif game_status == GameStatus.DRAW:
        print("Draw!")
    elif game_status == GameStatus.MATCH_ABORTED:
        print("Match aborted.")
    else:
        print("Unknown game status:", game_status)


def evaluateOps(board):
    # print("opps be like")
    n = ops_number
    recommended = Move(0,0,0,0)

    for i in range(len(board)):
        consecutive = 0
        for j in range(len(board[i])):
            if(board[i][j] == n):
                consecutive += 1
                # print( consecutive,"consecutive at ", i)
            if(board[i][j] == 0):
                consecutive = 0
            if(consecutive == 3):
                if(j + 1 < 5 and board[i][j+1] == 0):
                    recommended.x = j + 1
                    recommended.y = i
                    return recommended
                if(board[i][j - 3] == 0):
                    recommended.x = j -3
                    recommended.y = i
                    return recommended
                print("recommended", recommended.x, recommended.y)
                
    print()
            
    for j in range(len(board[0])):
        consecutive = 0
        for i in range(len(board)):
            if(board[i][j] == n):
                consecutive += 1
                # print( consecutive,"vert consecutive at ", i)
            if(board[i][j] == 0):
                consecutive = 0
            if(consecutive == 2):
                print("2 consec vertical")
                if(j+1 < 5 and board[i][j + 1] == n):
                    print("square")
                    recommended.x = j+1
                    recommended.y = i-1
                    # print("square recommended", recommended.x, recommended.y)
                    return recommended
                if(j- 1 > 0 and board[i][j - 1] == n):   #REMEMBER NULL CHECKS
                   print("square")
                   recommended.x = j-1
                   recommended.y = i
                #    print("square recommended", recommended.x, recommended.y)
                   return recommended
                if(i-1 > 0 and j+1 <5 and board[i-1][j + 1] == n):
                    print("square")
                    recommended.x = j+1
                    recommended.y = i
                    # print("square recommended", recommended.x, recommended.y)
                    return recommended
                if(i-1>0 and j - 1 > 0 and board[i-1][j - 1] == n):
                    print("square")
                    recommended.x = j-1
                    recommended.y = i
                    # print("square recommended", recommended.x, recommended.y)
                    return recommended

            if(consecutive == 3):
                if(i +1 <= 5 and board[i+1][j]):
                    # recommended = (i + 1, j)
                    recommended.x = j
                    recommended.y = i + 1
                    print("vertical recommended", recommended.x, recommended.y)
                    return recommended
                elif (i-3>0 and board[i-3][j] == 0):
                    # recommended = (i - 3, j)
                    recommended.x = j
                    recommended.y  = i-3
                    print("vertical recommended", recommended.x, recommended.y)
                    return recommended
                
                    
    return False

def calculate_distance(point1, point2):
    return math.sqrt((point2[0] - point1[0])**2 + (point2[1] - point1[1])**2)

def non_aligned_square(board):
    #find if 3 points make a right-isocles triangle, if so compute 4th point a recommend as move
    n = ops_number
    ops_positions = []
    for i in range(len(board)):
        for j in range(len(board[i])):
            if(board[i][j] == n):
                ops_positions.append((i,j))

    a = calculate_distance(ops_positions[0], ops_positions[1])
    b = calculate_distance(ops_positions[1], ops_positions[2])
    c = calculate_distance(ops_positions[2], ops_positions[0])

    right_angle = (a**2 + b**2 == c**2) or (b**2 + c**2 == a**2) or (c**2 + a**2 == b**2)

    isoceles = (a == b) or (b == c) or (c == a)

    if(not right_angle or not isoceles):
        return False
    
    
                       


def squareCheck(board, given_x, given_y, value):
    #check square upper right
    x = given_x
    y = given_y
    temp_value = 1
    n = player_number
    if y+1 < 5 and board[y+1][x] == n:
        temp_value += 1
    if  y+1 < 5 and x+1 < 5 and board[y+1][x+1] == n:
        temp_value += 1

    if  x+1 < 5 and board[y][x+1] == n:
        temp_value += 1

    #chek square lower right
    x = given_x
    y = given_y
    temp_value = 1
    if x+1 < 5 and board[y][x+1] == n:
        temp_value += 1
    if y-1 >=0  and x+1 < 5 and board [y-1][x+1] == n:
        temp_value += 1
    if y-1 >= 0 and board [y-1][x] == n:
        temp_value += 1

    if temp_value > value:
        value = temp_value

    #check square lower left
    x = given_x
    y = given_y
    temp_value = 1

    if x-1 >= 0 and board[y][x-1] == n:
        temp_value += 1
    if y-1 >=0 and x-1>=0 and board [y-1][x-1] == n:
        temp_value += 1
    if y-1 >= 0 and board [y-1][x] == n:
        temp_value += 1

    if temp_value > value:
        value = temp_value

    #check upper left
    x = given_x
    y = given_y
    n = player_number
    if y+1 < 5 and board[y+1][x] == n:
        value += 1
    if y+1 <5 and x-1 >= 0 and board[y+1][x-1] == n:
        value += 1

    if x-1 >= 0 and board[y][x-1] == n:
        value += 1

    if temp_value > value:
            value = temp_value

    return value


    
    
    


def evaluateMove(board, given_x, given_y, stone):
    value = 1
    n = player_number
    # print("player number" ,player_number)
    
    x = given_x
    y = given_y
    while x + 1 < 5 and board[y][x+1] == n and board[y][x+1] != board[stone[1]][stone[0]]:
        value += 1
        x += 1

    x = given_x
    y = given_y
    temp_value = 1
    while x - 1 >= 0 and board[y][x - 1] == n and board[y][x-1] != board[stone[1]][stone[0]]:
        x -= 1
        temp_value += 1

    if temp_value > value:
        value = temp_value

    x = given_x
    y = given_y
    temp_value = 1
    while  y + 1 < 5 and  board[y + 1][x] == n and board[y+1][x] != board[stone[1]][stone[0]]:
        y += 1
        temp_value += 1

    if temp_value > value:
        value = temp_value

    x = given_x
    y = given_y
    temp_value = 1
    while  y - 1 >= 0 and  board[y - 1][x] == n and board[y-1][x] != board[stone[1]][stone[0]]:
        y -= 1
        temp_value += 1

    if temp_value > value:
        value = temp_value

    value = squareCheck(board, given_x, given_y, value)

    # print("move to ", given_x , given_y, "has value", value)


    #return move object (s,x,y,v)
    return value

# Helper
def isTurnPlayable(game_status):
    return game_status == YOUR_TURN


# Query, calculate, submit, repeat, query, calculate, submit, repeat
def autoPlay():
    count = 0
    occupied_fields = []
    while True:
        state, status = showGameState()
        #declare list of struct containing x,y postitions and value of a possible (legal) move,
        #the move with the highest value will be selected
        moves_list = []

        print(status)
        if isMatchOver(status):
            print("breaking because match over")
            printMatchOverMessage(status)
            break
        if not isTurnPlayable(status):
            time.sleep(1)
            continue

        width = 5
        height = 5
        byte_board = state.board
        board = []
        for y in range(height):
            row = []
            for x in range(width):
               row.append(byte_board[x + y * width])#adding the board data to the row

            board.append(row) #adding the row to the board
             #board[x][y] = byte_board[x + y * width]
      
            

        #count = sum(row.count('1') for row in board) #counting the number of 1's in the board
        
        if count == 4: #if there are 4 1's in the board then we have to move one
            for stone in occupied_fields:
                print ("stone at ",stone[0], stone[1])
                x = stone[0]
                y = stone[1]
                if x+1 < 5 and board [y][x+1] == 0 :
                   # print(board[x+1][y], "THIS")
                    x1 = x
                    y1 = y
                    x2 = x+1
                    y2 = y
                    # print("free space on the right")
                    # occupied_fields.remove((stone[0], stone[1])) # we call calcValue() here 
                    # occupied_fields.append((x+1, y))
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    break
                if  x+1 < 5 and y-1 >= 0 and board[y-1][x+1] == 0:
                    x1 = x
                    y1 = y
                    x2 = x+1
                    y2 = y-1
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    #occupied_fields.remove((stone[0], stone[1]))  # we call calcValue() here 
                    #occupied_fields.append((x+1, y-1))
                    #break
                if  y-1 >= 0 and board[y-1][x] == 0:
                    x1 = x
                    y1 = y
                    x2 = x
                    y2 = y-1
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    # occupied_fields.remove((stone[0], stone[1])) # we call calcValue() here  et...
                    # occupied_fields.append((x, y-1))
                    # break
                if x-1 >= 0 and y-1 >= 0 and board [y-1][x-1] == 0:
                    x1 = x
                    y1 = y
                    x2 = x-1
                    y2 = y-1
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    # occupied_fields.remove((stone[0], stone[1]))
                    # occupied_fields.append((x-1, y-1))
                    # break
                if x-1 >= 0 and board [y][x-1] == 0:
                    x1 = x
                    y1 = y
                    x2 = x-1
                    y2 = y
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    # occupied_fields.remove((stone[0], stone[1]))
                    # occupied_fields.append((x-1, y))
                    # break
                if  x-1 >= 0 and y+1 < 5 and board [y+1][x-1] == 0:
                    x1 = x
                    y1 = y
                    x2 = x-1
                    y2 = y+1
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    # occupied_fields.remove((stone[0], stone[1]))
                    # occupied_fields.append((x-1, y+1))
                    # break
                if y+1 < 5 and board [y+1][x] == 0:
                    x1 = x
                    y1 = y
                    x2 = x
                    y2 = y+1
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    # occupied_fields.remove((stone[0], stone[1]))
                    # occupied_fields.append((x, y+1))
                    # break
                if x+1 < 5 and y+1 < 5 and  board [y+1][x+1] == 0:
                    x1 = x
                    y1 = y
                    x2 = x+1
                    y2 = y+1
                    value = evaluateMove(board, x2, y2, stone)
                    move = Move(stone, x2, y2, value)
                    moves_list.append(move)
                    # occupied_fields.remove((stone[0], stone[1]))
                    # occupied_fields.append((x+1, y+1))
                    # break

            #choose highest value and update stone position
            chosen_move = max(moves_list, key=lambda x: x.value)
            occupied_fields.remove((chosen_move.stone[0], chosen_move.stone[1]))
            occupied_fields.append((chosen_move.x, chosen_move.y))

            print(chosen_move.stone[0],chosen_move.stone[1], chosen_move.x,chosen_move.y, "inside if")
            submitTurn({"x1": chosen_move.stone[0], "y1": chosen_move.stone[1], "x2": chosen_move.x, "y2": chosen_move.y})
        else:
            worth_it = True
           
            freex = -1
            freey = -1
            for y in range(height):
                for x in range(width):
                    if board[y][x] == 0:
                        stone = [x,y]
                        value = evaluateMove(board, x, y,stone )
                        move = Move(0, x, y, value)
                        moves_list.append(move)
                        # if(count == 3):
                        #     evaluateOps(board)
                       # break
            chosen_move = max(moves_list, key=lambda x: x.value)
            
            possible_move = evaluateOps(board)
            if(possible_move):
                if board[possible_move.y][possible_move.x] == 0:
                    chosen_move = possible_move

            
            # if(count == 0 and board[3][0] == 0 and worth_it):
            #     chosen_move.x = 0
            #     chosen_move.y = 3
            # elif (board[3][0] != 0):
            #     worth_it = False 
            # if(count == 1 and board[4][1] == 0 and worth_it):
            #     chosen_move.x = 1
            #     chosen_move.y = 4
            # elif (board[4][1] != 0):
            #     worth_it = False
            # if(count == 2 and board[2][1] == 0 and worth_it):
            #     chosen_move.x = 1
            #     chosen_move.y = 2
            # elif(board[2][1] != 0):
            #     worth_it = False
            # if(count == 3 and board[3][2] == 0 and worth_it):
            #     chosen_move.x = 2
            #     chosen_move.y = 3
            # elif(board [3][2]!=0 ) :
            #     worth_it = False
            
            
            print(chosen_move.x, chosen_move.y)
            print("inside else")
            submitTurn({"x1": 0, "y1": 0, "x2": chosen_move.x, "y2": chosen_move.y}) # cannot pass a -1 here
            count += 1
            occupied_fields.append((chosen_move.x, chosen_move.y))
            print(occupied_fields)

   #print("TODO: Implement autoPlay()")


   

def main():
    print("UserToken:", userToken)
    newMatch(5)
    waitMatchStarted()
    print("Opponent found.")
    queryOpponentInfo()
    queryTimeout()
    autoPlay()


if __name__ == "__main__":
    main()
