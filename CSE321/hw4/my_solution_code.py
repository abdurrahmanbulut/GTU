# Abdurrahman Bulut
# abdurrahmanbulut08@gmail.com
# 1901042258
import random

#------------------------------------------------------
##Question 1 start
#------------------------------------------------------

def find_maximum_points(board):
  max_points = 0
  path = []
  print("board: ", board)
  print("\n")
  n = len(board)
  m = len(board[0])

  # Try every possible route starting from A1 B1
  for i in range(n):
    for j in range(m):
      route = [(0, 0)]
      points = board[0][0]

      # Move to the next point
      x, y = 0, 0
      while x < n-1 or y < m-1:
        if x < n-1 and y < m-1:  # Both points are within bounds
          if board[x+1][y] > board[x][y+1]:  # Choose the coordinate with the higher point value
            x += 1
          else:
            y += 1
        elif x < n-1:  # Only x coordinate is within bounds
          x += 1
        elif y < m-1:  # Only y coordinate is within bounds
          y += 1
        route.append((x, y))
        points += board[x][y]

      # Update the maximum points and route if necessary
      if points > max_points:
        max_points = points
        path = route
  
  #Print the routes as in the assignment
  print(f"Route: {' → '.join(f'A{i+1} B{j+1}' for i, j in route)}")

  # Print the maximum points like in the example
  print("Points: ", end="")
  for i, coord in enumerate(path):
    x, y = coord
    if i > 0:
      print(" + ", end="")
    print(board[x][y], end="")
  print(" =", max_points)

  # We can also return this results if it will be asked.


#------------------------------------------------------
##1 end
#------------------------------------------------------
#------------------------------------------------------
#2 start
#------------------------------------------------------
def find_median(lst):
	# base case: if the array has 0 or 1 elements, the median is the only element
    if len(lst) == 0:
        return None
    if len(lst) == 1:
        return lst[0]

    if len(lst) % 2 == 1:  # odd number of items
        return quick_select(lst, len(lst) / 2)
    else:
        return (quick_select(lst, len(lst) / 2 - 1) + quick_select(lst, len(lst) / 2)) / 2


def quick_select(lst, k):
	# base case: if the array has 0 or 1 elements, the median is the only element
    if len(lst) == 0:
        return None
    if len(lst) == 1:
        return lst[0]

    pivot_index = random.randint(0, len(lst) - 1)  # picks a pivot
    pivot = lst[pivot_index]

    smallers = [item for item in lst if item < pivot]
    highers = [item for item in lst if item > pivot]
    pivots = [item for item in lst if item == pivot]

    if k < len(smallers):
        return quick_select(smallers, k)
    elif k < len(smallers) + len(pivots):
        return pivots[0]
    else:
        return quick_select(highers, k - len(smallers) - len(pivots))


#------------------------------------------------------
##2 end
#------------------------------------------------------
#------------------------------------------------------
##3.1 start
#------------------------------------------------------
class Node:
  def __init__(self, player):
    self.player = player
    self.next = None

class CircularLinkedList:
  def __init__(self, n):
    self.head = None
    self.size = 0

    # Create a circular linked list with n players
    for i in range(1, n+1):
      node = Node(f"P{i}")
      if self.head is None:
        self.head = node
        node.next = node
      else:
        current = self.head
        while current.next != self.head:
          current = current.next
        current.next = node
        node.next = self.head
    self.size = n

  def find_winner(self):
    # Initialize a pointer to the first node (P1)
    pointer = self.head

    # While there is more than one node in the list
    while self.size > 1:
      # Remove the node to the left of the pointer (the nearest player on the left)
      pointer.next = pointer.next.next
      self.size -= 1

      # Move the pointer to the node on the right of the removed node
      pointer = pointer.next

    # Return the remaining node as the winner
    return pointer.player


#------------------------------------------------------
##3.1 end
#------------------------------------------------------
##-----------------------------------------------------
##3.2 start
#------------------------------------------------------
## It returns number of player - 1
def find_winner(n):

    if n <= 1:
        return 0
    else:
        return (find_winner(n - 1) + 2) % n

##-----------------------------------------------------
##3.2 end
#------------------------------------------------------

###################### Driver funcs and Menu #####################
print("\n************cse321 Homework 4 - Driver Function****************\n")

def driver_q1():

    print("**********Question 1 Start********\n")

    print("set the size of the board")
    # To make n or m can cause empty array problems. So, it should start from 1
    print("n created by randomly. n is between 1 and 10")
    n = random.randint(1, 10)
    print("m created by randomly. m is between 1 and 10")
    m = random.randint(1, 10)

    # create the board and fill it with random values
    print("board created by randomly. numbers are between 1 and 100")
    board = [[random.randint(1, 100) for j in range(m)] for i in range(n)]  
    print("\nn:", n,", ", "m:", m)  
    find_maximum_points(board)

    print("\n**********Question 1 End********\n")



def driver_q2():
    print("******************** Question 2 Start ****************************\n")

    print("set the size of the array randomly")
    size = random.randint(0, 100)

    print("create the array and fill it with random values")
    board = [random.randint(0, 100) for i in range(size)]
    print("Median of (randomly created)", board, ": ", find_median(board))
    print("Median of [1, 2, 3, 4, 5]: ",find_median([1, 2, 3, 4, 5]))
    print("Median of [5, 4, 3, 2, 1]: ",find_median([5, 4, 3, 2, 1])) 
    print("Median of [1, 3, 5, 2, 7, 1]: ",find_median([1, 3, 5, 2, 7, 1]))  
    print("Median of [1, 2, 3, 4, 5, 6]: ",find_median([1, 2, 3, 4, 5, 6])) 
    print("Median of [10, 52, 44, 13, 25, 55]: ",find_median([10, 52, 44, 13, 25, 55]))  
    print("Median of [1]: ",find_median([1]))
    print("Median of []: ",find_median([])) 

    print("******************** Question 2 End **********************\n")


def driver_q3():
    print("******************** Question 3 Start ****************************\n"),
    print("a) Using circular linked list")

    print("Determined number of the players randomly between 0-100")
    numberOfPlayer = random.randint(0, 100)

    print("Creating circular linked list for ", numberOfPlayer, " player")
    cll = CircularLinkedList(numberOfPlayer)
    print("winner is determined")
    winner = cll.find_winner()
    print(winner)

	# 5 players
    print("Creating circular linked list for 5 player")
    cll = CircularLinkedList(5)
    print("winner is determined")
    winner = cll.find_winner()
    print(winner)

	# 6 players
    print("Creating circular linked list for 6 player")
    cll = CircularLinkedList(6)
    print("winner is determined")
    winner = cll.find_winner()
    print(winner) 

	# 7 players
    print("Creating circular linked list for 7 player")
    cll = CircularLinkedList(7)
    print("winner is determined")
    winner = cll.find_winner()
    print(winner)


    print("\nb) Using decrease-and-conquer")

    print("Select a random number to represent number of player")
    num_of_player = random.randint(1, 9)
    print("Number of player: ", num_of_player)
    print(find_winner(num_of_player) + 1)

    print("\n******************* Question 3 End ****************************\n")


def main():
    print("**********Main function Start***********\n")
    print("Driver for question 1 called")
    driver_q1()
    print("Driver for question 2 called")
    driver_q2()
    print("Driver for question 3 called")
    driver_q3()
   
    print("**********Main function End***********\n")
    print("**********Homework 4 End***********\n")


if __name__ == "__main__":
    main()


