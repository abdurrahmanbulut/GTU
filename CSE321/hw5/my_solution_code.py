# Abdurrahman Bulut
# abdurrahmanbulut08@gmail.com
# 1901042258
import random

#------------------------------------------------------
##Question 1 start
#------------------------------------------------------

def longest_common_substring(words, start, end):
  # base case
  if start > end:
    return ""

  # check if the character at the middle index is the same in all the strings
  middle = (start + end) // 2
  for i in range(1, len(words)):
    if words[0][middle] != words[i][middle]:
      # return the longest common substring in the first half of the substring if characters are not the same
      return longest_common_substring(words, start, middle - 1)

  # return the longest substring in the second half of the substring if characters are the same
  return words[0][start:middle + 1] + longest_common_substring(words, middle + 1, end)

#------------------------------------------------------
##1 end
#------------------------------------------------------
#------------------------------------------------------
#2.a start
#------------------------------------------------------
def max_profit_a(prices):
  # base case
  if len(prices) == 0 or len(prices) == 1:
    return 0

  # divide the list of prices into two halves
  mid = len(prices) // 2
  left_prices = prices[:mid]
  right_prices = prices[mid:]

  # recursively find the maximum profit by buying and selling in the left and right halves
  left_profit = max_profit_a(left_prices)
  right_profit = max_profit_a(right_prices)

  # find the maximum profit by buying in the left half and selling in the right half
  min_left_price = min(left_prices)
  max_right_price = max(right_prices)
  cross_profit = max_right_price - min_left_price

  # return the maximum of the profits from the left and right halves and the cross profit
  return max(left_profit, right_profit, cross_profit)


#------------------------------------------------------
##2.a end
#------------------------------------------------------
#------------------------------------------------------
#2.b start
#------------------------------------------------------
def max_profit_b(prices):
  # base case: if there are no prices or only one price, there is no profit to be made
  if len(prices) == 0 or len(prices) == 1:
    return 0

  # initialize the minimum price and maximum profit to the first price in the list
  min_price = prices[0]
  max_profit = 0

  # iterate through the list of prices
  for i in range(1, len(prices)):
    # update the minimum price if the current price is lower
    min_price = min(min_price, prices[i])
    # update the maximum profit if it can be increased by selling at the current price
    max_profit = max(max_profit, prices[i] - min_price)

  return max_profit

#------------------------------------------------------
##2.b end
#------------------------------------------------------
#------------------------------------------------------
##3 start
#------------------------------------------------------
def longest_increasing_subarray(arr):
   
    n = len(arr)

    # Initialize the DP array to store the length of the longest increasing subarray ending at each index
    dp = [1] * n

    # Initialize the start array to store the starting index of the longest increasing subarray ending at each index
    start = [0] * n

    # Iterate through the array, starting from the second element
    for i in range(1, n):
        # it can be added to the longest increasing subarray ending at the previous element
        if arr[i] > arr[i-1]:
            # Update the length of the longest increasing subarray ending at the current index
            dp[i] = dp[i-1] + 1
            # Update the starting index of the longest increasing subarray ending at the current index
            start[i] = start[i-1]
        else:
            # If the current element is not greater than the previous element,
            # it cannot be added to the longest increasing subarray ending at the previous element
            # and must start a new subarray
            start[i] = i

    # Find the maximum length of the longest increasing subarrays
    maxLen = max(dp)
    # Find the index at which the longest increasing subarray with the maximum length ends
    end = dp.index(maxLen)
    # Return the longest increasing subarray
    return arr[start[end]:end+1]

#------------------------------------------------------
##3 end
#------------------------------------------------------
##-----------------------------------------------------
##-----------------------------------------------------
##4.a start
#------------------------------------------------------
def highest_score_dynamic(n, m, map):
  # Create a 2D array to store the maximum score at each coordinate
  scores = [[0 for _ in range(m)] for _ in range(n)]

  # Set the initial score at the starting coordinate
  scores[0][0] = map[0][0]

  for i in range(n):
    for j in range(m):
      # If we are not at the starting coordinate, set the score to the maximum of the
      # scores of the two possible previous coordinates plus the score at the current coordinate
      if i > 0 or j > 0:
        scores[i][j] = max(scores[i-1][j], scores[i][j-1]) + map[i][j]

  # Return the score at the final coordinate
  return scores[n-1][m-1]

##-----------------------------------------------------
##4.a end
#------------------------------------------------------
##-----------------------------------------------------
##4.b start
#------------------------------------------------------
def highest_score_greedy(n, m, map):
  # Set the initial coordinate and score
  i = 0
  j = 0
  score = map[i][j]

  # Keep track of the path taken
  path = [(i, j)]

  # Continue until we reach the final coordinate
  while i < n-1 or j < m-1:
    # Check if moving down or right gives a higher score
    if i < n-1 and (j == m-1 or map[i+1][j] > map[i][j+1]):
      i += 1
    else:
      j += 1
    score += map[i][j]
    path.append((i, j))

  return score
##-----------------------------------------------------
##4.b end
#------------------------------------------------------


###################### Driver funcs and Menu #####################
print("\n************cse321 Homework 5 - Driver Function****************\n")

def driver_q1():

    print("**********Question 1 Start********\n")

	# test the function with the example input
    print("test the function with the example input")
    
    print("Example Input 1: ['programmable', 'programming', 'programmer', 'programmatic', 'programmability']")
    strings = ["programmable", "programming", "programmer", "programmatic", "programmability"]
    print("Output: ")
    print("Length: ", len(longest_common_substring(strings, 0, len(strings[0]) - 1)))
    print("Sub-array: ", longest_common_substring(strings, 0, len(strings[0]) - 1))

    print("\nExample Input 2: ['compute', 'compatible', 'computer', 'compare', 'compactness']")
    strings = ["compute", "compatible", "computer", "compare", "compactness"]
    print("Output: ")
    print("Length: ", len(longest_common_substring(strings, 0, len(strings[0]) - 1)))
    print("Sub-array: ", longest_common_substring(strings, 0, len(strings[0]) - 1))

    print("\n**********Question 1 End********\n")


def driver_q2():
    print("******************** Question 2 Start ****************************\n")
    print("a) Divide-and-conquer algorithm")


    print("Prices: [10, 11, 10, 9, 8, 7, 9, 11]")
    prices1 = [10, 11, 10, 9, 8, 7, 9, 11]
    print("Max profit: ", max_profit_a(prices1))  

    print("\nPrices: [100, 110, 80, 90, 110, 70, 80, 80, 90]")
    prices2 = [100, 110, 80, 90, 110, 70, 80, 80, 90]
    print("Max profit: ", max_profit_a(prices2))

    print("\nPrices: [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]")
    prices3 = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
    print("Max profit: ", max_profit_a(prices3)) 

    print("\nPrices: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]\n")
    prices4 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    print("Max profit: ", max_profit_a(prices4))  


    print("set the size of the array randomly")
    size = random.randint(0, 100)

    print("create the array and fill it with random values")
    prices5 = [random.randint(0, 100) for i in range(size)]
    print("Prices: ", prices5)
    print("Max profit: ", max_profit_a(prices5))


    print("\nb) Algorithm that is not based on the divide-and-conquer approach in linear time.")
    print("Prices: [10, 11, 10, 9, 8, 7, 9, 11]")
    prices1 = [10, 11, 10, 9, 8, 7, 9, 11]
    print("Max profit: ", max_profit_b(prices1))  

    print("\nPrices: [100, 110, 80, 90, 110, 70, 80, 80, 90]")
    prices2 = [100, 110, 80, 90, 110, 70, 80, 80, 90]
    print("Max profit: ", max_profit_b(prices2))

    print("\nPrices: [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]")
    prices3 = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
    print("Max profit: ", max_profit_b(prices3)) 

    print("\nPrices: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]")
    prices4 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    print("Max profit: ", max_profit_b(prices4))  


    print("\nset the size of the array randomly")
    size = random.randint(0, 100)

    print("create the array and fill it with random values")
    prices5 = [random.randint(0, 100) for i in range(size)]
    print("Prices: ", prices5)
    print(max_profit_b(prices5))

    print("******************** Question 2 End **********************\n")


def driver_q3():
    print("******************** Question 3 Start ****************************\n"),
		
    print("Input array is: [1, 4, 5, 2, 4, 3, 6, 7, 1, 2, 3, 4, 7]")
    arr = [1, 4, 5, 2, 4, 3, 6, 7, 1, 2, 3, 4, 7]
    result = longest_increasing_subarray(arr)
    print("Output", result)

    print("\nInput array is: [1, 2, 3, 4, 1, 2, 3, 5, 2, 3, 4]")
    arr = [1, 2, 3, 4, 1, 2, 3, 5, 2, 3, 4]
    result = longest_increasing_subarray(arr)
    print("Output", result)

    print("\nInput array is: [5, 6, 3, 2, 1]")
    arr = [5, 6, 3, 2, 1]
    result = longest_increasing_subarray(arr)
    print("Output", result)


    print("\nset the size of the array randomly")
    size = random.randint(0, 100)

    print("create the array and fill it with random values")
    arr = [random.randint(0, 100) for i in range(size)]
    result = longest_increasing_subarray(arr)
    print("Input array is: ", arr)
    print("Output", result)

    print("\n******************* Question 3 End ****************************\n")


def driver_q4():
    print("******************** Question 4 Start ****************************\n"),
    print("a) Dynamic programming algorithm")
    
    # To make n or m can cause empty array problems. So, it should start from 1
    print("n created by randomly. n is between 1 and 10")
    n = random.randint(1, 10)
    print("m created by randomly. m is between 1 and 10")
    m = random.randint(1, 10)

    # create the board and fill it with random values
    print("board created by randomly. numbers are between 1 and 100")
    board = [[random.randint(1, 100) for j in range(m)] for i in range(n)]  
    print("board: ", board)
    print("\nn:", n,", ", "m:", m)  
    result = highest_score_dynamic(n, m, board)

    print("Max points: ", result)
    
    
    print("\nb) Greedy algorithm")
      # To make n or m can cause empty array problems. So, it should start from 1
    print("n created by randomly. n is between 1 and 10")
    n = random.randint(1, 10)
    print("m created by randomly. m is between 1 and 10")
    m = random.randint(1, 10)

    # create the board and fill it with random values
    print("board created by randomly. numbers are between 1 and 100")
    board = [[random.randint(1, 100) for j in range(m)] for i in range(n)]  
    print("board: ", board)
    print("\nn:", n,", ", "m:", m)  
    result = highest_score_greedy(n, m, board)

    print("Max points: ", result)

    highest_score_greedy(n, m, board)

    print("\n******************* Question 4 End ****************************\n")


def main():
    print("**********Main function Start***********\n")
    print("Driver for question 1 called")
    driver_q1()
    print("Driver for question 2 called")
    driver_q2()
    print("Driver for question 3 called")
    driver_q3()
    print("Driver for question 4 called")
    driver_q4()
   
    print("**********Main function End***********\n")
    print("**********Homework 5 End***********\n")


if __name__ == "__main__":
    main()


