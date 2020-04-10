
# Advanced programming Lab 8

- Compulsory, all tasks
- Optional, all tasks
- Bonus, all tasks

## Project packages:

- command: contains command classes (that contain the solution for some Optional/Bonus tasks)
- common: contains static methods used in differents parts of the application (it's created in order to avoid duplicate code)
- database.connection: contains the Singleton Database class and the connection pool class
- database.dao: contains the DAO classes
- exception: contains the custom exceptions user in the application
- model: contains the object-oriented model of the database 
- shell: contains the logic for the main shell of the program

## Details about the application:

### Database

I used MySQL and I created the following tables:

- artists(id,name,country)
- albums(id,name,artist_id,year)
- charts(id,name)
- charts_albums(id,rank,chart_id,album_id))

### HTML Report

I used https://velocity.apache.org/

### Connection pool

I used HikariCP, https://github.com/brettwooldridge/HikariCP

### Artists ranking

There can be multiple charts. For a chart, the first ranked artist gets 3 points, the second ranked artist gets 2 points, the third ranked artist gets 3 points, and the others are grouped in blocks of 10 (positions 4 - 13, 14 - 23, ...) and get 1.0 - blockNumber * 0.1 (the artists on positions 4-13 receive 0.9 points, ...). The number of points cannot go below 0.2 (so all the artists ranked 84 or higher will get 0.2 points).
                                  
## Demo: https://youtu.be/h_qZcNyx_ts
