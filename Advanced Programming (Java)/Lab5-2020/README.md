
# Advanced programming Lab 5

- Compulsory, all tasks
- Optional, all tasks
- Bonus, all tasks

The implemented shell commands:

A command can be executed by typing its name or its index (for the first command, "catalog" or "1")
         
 1. catalog                                      (displays information about the active catalog)
 2. create            catalogName                (creates a new catalog)
 3. add-path          documentName documentPath  (adds a file to the catalog)
 4. add-url           documentName documentUrl   (adds an external file to the catalog)
 5. update-tags       documentName               (enters an interactive menu where document tags can be updated)
 6. view              documentName               (opens a file, using the native operating system application)
 7. report            { html/ pdf/ word }        (creates a report in the specified format)
 8. info              documentName               (displays metadata about a file)
 9. save              fileName                   (saves the active catalog to a file, using a plain text representation)
10. save-serialized   fileName                   (saves the active catalog to a file, using object serialization) 
11. load              fileName                   (loads a catalog from a text file)
12. load-serialized   fileName                   (loads a catalog from a binary file)
13. list-cmd                                     (lists all the available commands)
14. quit                                         (stops the application)

Demo: https://youtu.be/k4OdyGQPdkc
