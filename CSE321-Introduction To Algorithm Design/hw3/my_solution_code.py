from collections import defaultdict, deque

############### Question 1 #####################
################ part a ########################
def top_sort_dfs(graph):
  # Create a set of all nodes with course 0.
  nodes_with_course_0 = set([node for node in graph if graph[node]["course"] == 0])

  # Initialize an empty list of sorted nodes.
  sorted_nodes = []

  # While the set of nodes with course 0 is non-empty:
  while len(nodes_with_course_0) > 0:
    # Select a node with course 0 from the set.
    node = nodes_with_course_0.pop()

    # Remove the node from the set and add it to the list of sorted nodes.
    sorted_nodes.append(node)

    # Decrement the course of each of the node's children by 1.
    for child in graph[node]["children"]:
      graph[child]["course"] -= 1

      # If a child's course becomes 0, add it to the set of nodes with course 0.
      if graph[child]["course"] == 0:
        nodes_with_course_0.add(child)

  # If the list of sorted nodes contains all of the nodes in the graph, return the list as the topological ordering.
  # Otherwise, the graph contains a cycle and no topological ordering is possible.
  if len(sorted_nodes) == len(graph):
    return sorted_nodes
  else:
    return None


################ part b ########################
def top_sort_non_dfs(graph):
  # Initialize a queue of nodes with course 0.
  nodes_with_course_0 = deque([node for node in graph if graph[node]["course"] == 0])

  # Initialize an empty list of sorted nodes.
  sorted_nodes = []

  # While the queue of nodes with course 0 is non-empty:
  while len(nodes_with_course_0) > 0:
    # Select a node with course 0 from the queue.
    node = nodes_with_course_0.popleft()

    # Remove the node from the queue and add it to the list of sorted nodes.
    sorted_nodes.append(node)

    # Decrement the course of each of the node's children by 1.
    for child in graph[node]["children"]:
      graph[child]["course"] -= 1

      # If a child's course becomes 0, add it to the queue of nodes with course 0.
      if graph[child]["course"] == 0:
        nodes_with_course_0.append(child)

  # If the list of sorted nodes contains all of the nodes in the graph, return the list as the topological ordering.
  # Otherwise, the graph contains a cycle and no topological ordering is possible.
  if len(sorted_nodes) == len(graph):
    return sorted_nodes
  else:
    return None


################## Question 2 ######################
def pow_func(a, n):
  # base case
  if n == 0:
    return 1
  # a^n -> If n is even, it can be represented as (a^2)(n/2) and, if n is odd, as a * (a^2)((n-1)/2).
  if n % 2 == 0:
    return pow_func(a * a, n / 2)
  else:
    return a * pow_func(a * a, (n - 1) / 2)



################# Question 3 #######################
def solve_sudoku(board):
  # find empty spaces
  empty_spaces = [(i, j) for i in range(9) for j in range(9) if board[i][j] == 0]

  def is_valid(board, i, j, value):
  # check if the value is already present in the row, column, or 3x3 grid
   return not (value in board[i] or value in (row[j] for row in board)
              or value in board[i//3*3][j//3*3:j//3*3+3]
              or value in (board[i//3*3+k][j//3*3:j//3*3+3] for k in range(1, 3)))

  # recursive function to find solutions
  def find_solution(board, empty_spaces, index):
    # if no more empty spaces, we've found a solution
    if index >= len(empty_spaces):
      return True

    # get the next empty space
    i, j = empty_spaces[index]

    # try filling the empty space with all possible values
    for value in range(1, 10):
      # if the value is valid, fill the empty space and move to the next one
      if is_valid(board, i, j, value):
        board[i][j] = value
        if find_solution(board, empty_spaces, index + 1):
          return True

    # if we reach this point, it means that we have backtracked
    # and the previous value didn't lead to a solution, so we reset it
    board[i][j] = 0
    return False

  # start the search from the first empty space
  find_solution(board, empty_spaces, 0)



###################### Driver funcs and Menu #####################
print("\n************cse321 Homework 3 - Driver Function****************\n")

def driver_q1():
    print("**********Question 1 Start********")
    print("a) dfs solution")
    print("Creat a graph")
    graph = defaultdict(dict)

    print("Add nodes")
    for node in ["CSE102", "CSE241", "CSE222", "CSE321", "CSE422", "CSE211"]:
        graph[node]["course"] = 0
        graph[node]["children"] = []

    print("Add edges")
    graph["CSE102"]["children"].append("CSE241")
    graph["CSE241"]["course"] += 1

    graph["CSE241"]["children"].append("CSE222")
    graph["CSE222"]["course"] += 1

    graph["CSE222"]["children"].append("CSE321")
    graph["CSE321"]["course"] += 1

    graph["CSE321"]["children"].append("CSE422")
    graph["CSE422"]["course"] += 1

    graph["CSE211"]["children"].append("CSE321")
    graph["CSE321"]["course"] += 1

    print("Perform topological sort")
    sorted_nodes = top_sort_dfs(graph)

    print("Print the result")
    print(sorted_nodes)
    print("\n")

    print("b) non-dfs solution")
    print("Create a graph")
    graph = defaultdict(dict)

    print("Add nodes")
    for node in ["CSE102", "CSE241", "CSE222", "CSE321", "CSE422", "CSE211"]:
        graph[node]["course"] = 0
        graph[node]["children"] = []

    print("Add edges")
    graph["CSE102"]["children"].append("CSE241")
    graph["CSE241"]["course"] += 1

    graph["CSE241"]["children"].append("CSE222")
    graph["CSE222"]["course"] += 1

    graph["CSE222"]["children"].append("CSE321")
    graph["CSE321"]["course"] += 1

    graph["CSE321"]["children"].append("CSE422")
    graph["CSE422"]["course"] += 1

    graph["CSE211"]["children"].append("CSE321")
    graph["CSE321"]["course"] += 1

    print("Perform topological sort")
    sorted_nodes = top_sort_dfs(graph)

    print("Print the result")
    print(sorted_nodes)

    print("**********Question 1 End********\n")




def driver_q2():
    print("******************** Question 2 Start ****************************")
    print("Test 1, a = 2, n = 5")
    print("a^n = ", pow_func(2, 5))
    print("\nTest 2, a = -2, n = 5")
    print("a^n = ", pow_func(-2, 5))
    print("\nTest 3, a = 1, n = 5")
    print("a^n = ", pow_func(1, 5))
    print("\nTest 4, a = 0, n = 5")
    print("a^n = ", pow_func(0, 5))
    print("\nTest 5, a = 3, n = 0")
    print("a^n = ", pow_func(3, 0))
    print("******************** Question 2 End **********************\n")


def driver_q3():
    print("******************** Question 3 Start ****************************")
    print("Define 9x9 board\n")

    board =         [[3, 0, 6, 5, 0, 8, 4, 0, 0],
                    [5, 2, 0, 0, 0, 0, 0, 0, 0],
                    [0, 8, 7, 0, 0, 0, 0, 3, 1],
                    [0, 0, 3, 0, 1, 0, 0, 8, 0],
                    [9, 0, 0, 8, 6, 3, 0, 0, 5],
                    [0, 5, 0, 0, 9, 0, 6, 0, 0], 
                    [1, 3, 0, 0, 0, 0, 2, 5, 0],
                    [0, 0, 0, 0, 0, 0, 0, 7, 4],
                    [0, 0, 5, 2, 0, 6, 3, 0, 0] ]

    print("Input : ")
    
    for i in board:
        print(i)
    print("\nBoard created...\n")

    print("Calling to solve function to find a solution for the puzzle")
    solve_sudoku(board)
    print("Print solved Board\n")
    for i in board:
        print(i)
    print("\n******************* Question 3 End ****************************\n")

def menu():
    print("\nUser Test Menu\n")
    close_app = 1

    while close_app:
        flag = 0
        selection = 0
        while(flag == 0):
            print("Select a question:")
            print("1) You will be see an answer for given graph in homeqork. question 1 test is not available because you'll need to give a graph. So, test is not possible")
            print("2) question 2")
            print("3) question 3")
            print("4) Close")
            selection = int(input("Please Select 1, 2, 3 or 4: "))
            if selection == 1 or selection == 2 or selection == 3 or selection == 4:
                flag = 1

        if selection == 1:
            driver_q1()
            print("\n")

        elif selection == 2:
            print("Enter an 'a' number and a 'n' number. Calculation for a^n  where a ∈ Z and n ∈ Z+")
            
            a = int(input("Please Enter 'a': "))
            n = int(input("Please Enter 'n': "))

            if(n < 0):
                print("Invalid n value. It should be a positive integer number..")
                print("Going to the Menu")
                break
            
            print("Calculating...")
            result = pow_func(a, n)
            print("a^n is equals to : ", result)
            print("\n")
        
        elif selection == 3:
            print("Give 81 number for sudoku board. Give without spaces. Ex. 12345678998...")
            print("Sample solvable sudoku board: 070000009510420600080300700008001370023080040400900100962800030000010400700203096")
            print("You can just pass 81 zeros to test if you want to..")

            sudoku = input("Please enter: ")

            print(len(sudoku))            
            if len(sudoku) != 81:
                print("You should enter 81 number without spaces")
                continue
            
            board = [[int(sudoku[(i+j)-1]) for i in range(1,10)] for j in range(0,81,9)]
            print("\nBoard created...\n")
            for i in board:
                print(i)

            solve_sudoku(board)

            
            print("\nCalling to solve function to find a solution for the puzzle")
            print("Printing solved Board\n")
            for i in board:
                print(i)
            
            print("\n")

        elif selection == 4:
            close_app = 0
            print("Have a good day!\n")
        else:
            print("Invalid selection... Try again!")
            print("\n")

            break


def main():
    driver_q1()
    driver_q2()
    driver_q3()
    menu()
   

if __name__ == "__main__":
    main()
