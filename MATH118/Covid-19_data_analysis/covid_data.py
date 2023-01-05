# Abdurrahman Bulut
# 1901042258
# abdurrahmanbulut08@gmail.com

import csv
import numpy as np  # for variance


'''
2 inputs
pairwise_questions index - 
q3 index - 4
q4 index - 7
q11 index - 26
q14 index - 35
q15 index - 36
q16 index - 34
===============================
4 inputs
ave_min_max_var index - 16
q6 index - 17
q7 index - 19
q8 index - 21
q9 index - 23
q10 index - 25
q12 index - 31
q13 index - 32
'''

'''
read_file function
It takes filename which has covid informations
it reads informations of covid data
returns a list of datas without taking name of datas (basically, first row) ex. country, death, cases... 
'''
def read_file(file_name):
    with open(file_name, 'r') as csv_file:
        csv_reader = csv.reader(csv_file)
        i = 0
        data = []

        for row in csv_reader:
            if i == 0:  # i variable for skipping first row
                i += 1
            else:
                data.append(row)
        return data


'''
check_country_name_duplicate funcktion
it takes all countries name list and a country name 
returns true if consecutive two names are different 
'''
def check_country_name_duplicate(name_list, name):
    for i in name_list:
        if name == i:
            return False
    return True


'''
number_of_country function
it takes all information of covid data
returns the number of countries
'''
def number_of_country(covid_data):
    count = 0
    name_list = []

    for i in covid_data:
        if check_country_name_duplicate(name_list, i[2]):
            name_list.append(i[2])
            count += 1
    return count


'''
country_list function
it takes all informations of covid data
returns all country names list
'''
def country_list(covid_data):
    country_names = []
    country_names.append(covid_data[0][2])  # first name of the list for checking
    check_country_name = covid_data[0][2]   # checking consecutive names
    jump = 0

    for i in covid_data:
        if jump == 0:
            jump += 1
        else:
            if i[2] != check_country_name:
                country_names.append(i[2])
                check_country_name = i[2]
    return country_names


'''
earliest_date functions
it takes all informations of covid data
it returns earliest date and its country name
'''
def earliest_date(covid_data):
    j = 0
    index = 1
    result = []
    min_date = 2021*365 + 4*30 + 21  # Based on the day I wrote this code. If min date is smallest, the it'll be the earliest date

    for i in covid_data:
        if min_date > int(i[3].split('-')[0]) * 365 + int(i[3].split('-')[1]) * 30 + int(i[3].split('-')[2]):
            min_date = int(i[3].split('-')[0]) * 365 + int(i[3].split('-')[1]) * 30 + int(i[3].split('-')[2])
            index = j   # index for keeping name of country with looking date
        j += 1
    result.append([covid_data[index][2], covid_data[index][3]])
    return result

'''
pairwise_questions function
it takes all informations of covid data and index that's for questions of 3-4-11-14-15-16
returns country name and it's information which is wanted

'''
def pairwise_questions(covid_data, index):
    result = []
    flag = 0    # flag is for skipping first row whih is already taken
    list_non_empty = []  # to ignore empty cells

    for i in covid_data:
        if i[index] != '':
            list_non_empty.append([i[index], i[2]])

    name = list_non_empty[0][1]  # ilk sıradaki ülkenin ismini atıp kontrole başlıyor
    total = int(float(list_non_empty[0][0]))  # ilk bilgiyi  alıp kontrol sırasında karşılaştırıp son durumunu bulmak için

    for i in list_non_empty:
        if flag == 0:   # we initialized first row, so it's for skipping it
            flag += 1
        else:
            if name != i[1]:  # consecutive check
                result.append([name, total])
                name = i[1]
                total = 0
            else:
                if total < int(float(i[0])):
                    total = int(float(i[0]))

    result.append([name, total])
    return result


'''
ave_min_max_var function
it takes all informations of covid data and index for wanted column
it finds average, min value, max value and variation of each country and returns it
'''

def ave_min_max_var(covid_data, index):

    list_non_empty = []
    for_variance = []  # inludes each data of same countries and it will be using to find variance
    result = []

    for i in covid_data:  # to ignore empty cells
        if i[index] != '':
            list_non_empty.append([i[2], i[index]])

    name = list_non_empty[0][0]
    min_value = float(list_non_empty[0][1])
    max_value = min_value
    total = min_value  # it's initialized to first value and it will be using for average
    counter = 1
    for_variance.append(min_value)
    j = 1  # j is counting name index
    len_list = len(list_non_empty)

    while j < len_list:
        if list_non_empty[j][0] == name:
            if min_value > float(list_non_empty[j][1]):
                min_value = float(list_non_empty[j][1])
            if max_value < float(list_non_empty[j][1]):
                max_value = float(list_non_empty[j][1])
            total += float(list_non_empty[j][1])
            for_variance.append(float(list_non_empty[j][1]))
            counter += 1

        else:
            result.append([list_non_empty[j - 1][0], min_value, max_value, total / counter, np.var(for_variance)])
            min_value = float(list_non_empty[j][1])
            max_value = float(list_non_empty[j][1])
            total = float(list_non_empty[j][1])
            counter = 1
            for_variance.clear()
            for_variance.append(min_value)
            name = list_non_empty[j][0]

        j += 1

    result.append([list_non_empty[j - 1][0], min_value, max_value, total / counter, np.var(for_variance)])  # to take last country info
    return result


'''
q17 function
it takes all information of covid data to solving 17th question
it returns by a list using first data of each country
'''
def q17(covid_data):

    name = ""
    result = []
    jump = 0

    for line in covid_data:
        if jump == 0:   # jump lets initialize first name to name variable
            jump += 1
        elif line[2] != name:
            result.append([line[2], line[44], line[46], line[47], line[48], line[49], line[51], line[52],
                           line[53], line[54], line[55], line[56], line[57], line[58]])
            name = line[2]
    return result


'''
without_name function
it takes a list and a coutnry
it basically deletes name data all results of questions to expend last result
Since all function return name and its value, we need to delete name info
'''
def without_name(list_name, check_country):
    temp = []
    temp_list = []
    for i in list_name:
        if i[0] == check_country:
            i.pop(0)  # it deletes name part from list
            return i
        temp_list = i
    flag = 1
    while flag < len(temp_list):
        temp.append('')
        flag += 1
    return temp


'''
q18 function
it takes all information of covid data
returns first 17 question results as a list
'''
def q18(covid_data):
    result = []
    country_names = country_list(covid_data)
    final_result = []
    qu3 = pairwise_questions(covid_data, 4)
    qu4 = pairwise_questions(covid_data, 7)
    qu5 = ave_min_max_var(covid_data, 16)
    qu6 = ave_min_max_var(covid_data, 17)
    qu7 = ave_min_max_var(covid_data, 19)
    qu8 = ave_min_max_var(covid_data, 21)
    qu9 = ave_min_max_var(covid_data, 23)
    qu10 = ave_min_max_var(covid_data, 25)
    qu11 = pairwise_questions(covid_data, 26)
    qu12 = ave_min_max_var(covid_data, 31)
    qu13 = ave_min_max_var(covid_data, 32)
    qu14 = pairwise_questions(covid_data, 35)
    qu15 = pairwise_questions(covid_data, 36)
    qu16 = pairwise_questions(covid_data, 34)
    qu17 = q17(covid_data)
    j = 0
    len_country = len(country_names)
    while j < len_country:
        result.append(country_names[j])
        result.extend(without_name(qu3, country_names[j]))
        result.extend(without_name(qu4, country_names[j]))
        result.extend(without_name(qu5, country_names[j]))
        result.extend(without_name(qu6, country_names[j]))
        result.extend(without_name(qu7, country_names[j]))
        result.extend(without_name(qu8, country_names[j]))
        result.extend(without_name(qu9, country_names[j]))
        result.extend(without_name(qu10, country_names[j]))
        result.extend(without_name(qu11, country_names[j]))
        result.extend(without_name(qu12, country_names[j]))
        result.extend(without_name(qu13, country_names[j]))
        result.extend(without_name(qu14, country_names[j]))
        result.extend(without_name(qu15, country_names[j]))
        result.extend(without_name(qu16, country_names[j]))
        result.extend(without_name(qu17, country_names[j]))
        final_result.append(result[:])
        result.clear()
        j += 1
    return final_result

# Indexlere bakıp zaman kaybetmeyin diye yazdırma fonksiyonlarını ayrı ayrı yazdım.
# En altta yorumu kaldırarak fonksiyonları çalıştırabilirsiniz.
def write_q1(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        csv_file.writerow(['q1'])
        csv_file.writerow(['Number of countries'])
        csv_file.writerow([number_of_country(covid_data)])
        csv_file.writerow('')


def write_q2(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        csv_file.writerow(['q2'])
        csv_file.writerow(['Earliest country and date'])
        temp = earliest_date(covid_data)
        csv_file.writerow([temp[0], temp[1]])
        csv_file.writerow('')


def write_q3(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = pairwise_questions(covid_data, 4)
        csv_file.writerow(['q3'])
        csv_file.writerow(['Country', 'Case number'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q4(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = pairwise_questions(covid_data, 7)
        csv_file.writerow(['q4'])
        csv_file.writerow(['Country', 'Death number'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q5(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 16)
        csv_file.writerow(['q5'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q6(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 17)
        csv_file.writerow(['q6'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q7(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 19)
        csv_file.writerow(['q7'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q8(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 21)
        csv_file.writerow(['q8'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q9(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 23)
        csv_file.writerow(['q9'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q10(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 25)
        csv_file.writerow(['q10'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q11(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = pairwise_questions(covid_data, 26)
        csv_file.writerow(['q11'])
        csv_file.writerow(['Country', 'Test number'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q12(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 31)
        csv_file.writerow(['q12'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q13(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = ave_min_max_var(covid_data, 32)
        csv_file.writerow(['q13'])
        csv_file.writerow(['Country value', 'minimum value', 'maximum value', 'average value', 'variation value'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q14(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = pairwise_questions(covid_data, 35)
        csv_file.writerow(['q14'])
        csv_file.writerow(['Country', 'Vaccinated by at least one dose'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q15(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = pairwise_questions(covid_data, 36)
        csv_file.writerow(['q15'])
        csv_file.writerow(['Country', 'Vaccinated fully'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q16(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = pairwise_questions(covid_data, 34)
        csv_file.writerow(['q16'])
        csv_file.writerow(['Country', 'Vaccinations that administered'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q17(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = q17(covid_data)
        csv_file.writerow(['q17'])
        csv_file.writerow(['population', 'median_age', '#_of_people_aged_65_older',
        '#_of_people_aged_70_older', 'economic_performance', 'death_rates_due_to_heart_disease', 'diabetes_prevalence',
        '#_of_female_smokers', '#_of_male_smokers', 'handwashing_facilities', 'hospital_beds_per_thousand_people',
        'life_expectancy', 'human_development_index'])
        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


def write_q18(covid_data):
    with open('output.csv', mode='a', newline='') as csv_file:
        csv_file = csv.writer(csv_file, delimiter=',')
        temp = q18(covid_data)
        csv_file.writerow(['q18'])
        csv_file.writerow(['Country', 'q#3', 'q#4', 'q#5_min', 'q#5_max', 'q#5_avg', 'q#5_var', 'q#6_min', 'q#6_max', 'q#6_avg', 'q#6_var',
        'q#7_min', 'q#7_max', 'q#7_avg', 'q#7_var', 'q#8_min', 'q#8_max', 'q#8_avg', 'q#8_var', 'q#9_min', 'q#9_max', 'q#9_avg', 'q#9_var',
        'q#10_min', 'q#10_max', 'q#10_avg', 'q#10_var', 'q#11', 'q#12_min', 'q#12_max', 'q#12_avg', 'q#12_var',
        'q#13_min', 'q#13_max', 'q#13_avg', 'q#13_var', 'q#14', 'q#15', 'q#16', 'q#17_population', 'q#17_median_age', 'q#17_#_of_people_aged_65_older',
        'q#17_#_of_people_aged_70_older', 'q#17_economic_performance', 'q#17_death_rates_due_to_heart_disease', 'q#17_diabetes_prevalence',
        'q#17_#_of_female_smokers', 'q#17_#_of_male_smokers', 'q#17_handwashing_facilities', 'q#17_hospital_beds_per_thousand_people',
        'q#17_life_expectancy', 'q#17_human_development_index'])

        for i in temp:
            csv_file.writerow(i)
        csv_file.writerow('')


covid_data = read_file('owid-covid-data.csv')


write_q1(covid_data)
write_q2(covid_data)
write_q3(covid_data)
write_q4(covid_data)
write_q5(covid_data)
write_q6(covid_data)
write_q7(covid_data)
write_q8(covid_data)
write_q9(covid_data)
write_q10(covid_data)
write_q11(covid_data)
write_q12(covid_data)
write_q13(covid_data)
write_q14(covid_data)
write_q15(covid_data)
write_q16(covid_data)
write_q17(covid_data)
write_q18(covid_data)
