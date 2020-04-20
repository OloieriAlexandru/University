
# Advanced programming Lab 9

- Compulsory, all tasks
- Optional, all tasks
- Bonus, all tasks

## Project packages:

- app: contains the main class of the application
- command: contains command classes (that contain the solution for some Optional/Bonus tasks)
- entity: contains the persistent entities of the application
- graph: contains graph related classes
- repo.hibernate: contains the JPA repositories
- repo.jdbc.connection: contains the Singleton Database class and the connection pool class
- repo.jdbc.dao: contains a DAO class
- repo: contains the abstract factory class
- shell: contains the logic for the main shell of the program
- util: contains static methods used in differents parts of the application, and the "PersistenceUtil" class

## Details about the application:

### ORM

I used Hibernate

### The largest set of albums such that no two albums have the same artist or belong to the same genre

I built a bipartite graph and implemented a "Maximum bipartite matching" algorithm
                                  
## Demo: https://www.youtube.com/watch?v=Bu7RpX5nGqU
