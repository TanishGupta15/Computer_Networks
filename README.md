# Computer_Networks COL334 SEM I 2023-24 Assignment 2    
Creating a N client Distributed File System     


## Architecture        
1. N cleints will run thier receiver, broadcast, fetcher.    
2. Every cleint will keep receiving the messages from Server, which would be random lines of a file     
3. Aim is to create a file as soon as possible.   
4. Every Cleint will broadcast all the new lines it receives and simultaneously receives all the lines by others.   

## Running Code   
1. Make changes to the constants.h   
2. For every cleint:    
```    
make   
```   
3. For our assignment we were given a server to communicate with.   
