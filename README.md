# task04-noammirjani
# Explaining the containers in the code:

* First of all, the code is base on a solution we learned in OOP2 course.

Sorted data: The multimaps m_dataKeyX and m_dataKeyY are used to store cities sorted by their 
x and y coordinates, respectively. This allows for efficient range-based searches when searching 
for cities within a specific radius.

Efficient search operations: std::multimap provides logarithmic time complexity
for search and insert operations.This is beneficial for the various search operations
performed in the code, such as searching for cities by name or finding cities within a given radius.

Intersection of ranges: The getSquareRectCitiesMap function uses set intersection to find the
cities within a square range. std::multimap facilitates this operation by providing sorted input 
ranges, which is necessary for efficient intersection calculations.

While an std::unordered_multimap could also be used to store multiple cities with the same name,
it would not preserve the sorted order required for efficient range-based searches and intersection calculations. 
Thus, std::multimap is a suitable choice in this scenario.
