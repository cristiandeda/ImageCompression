# Image Compression using Quadtree

- developed during the **Data Structures** course in my first year at *University Politehnica of Bucharest*, *Faculty of Automatic Control and Computers*.

### Description

- task 1:
    - for the first task, the quadtree is built based on the information given in the `PPM file`
    - the `CompressArb` function builds the compression quadtree by calculating `RGB` and `mean` values
    - `mean` score is used to determine whether a color block still needs to be divided or not

- task 2:
    - for the second task, the compression file is generated based on the obtained quadtree
    - the compression tree is traversed in *BFS order* in order to obtain the proper values in the compression file
    - if the node is intern, `type 0` is written in the compression file
    - if the node is a leaf, the compression file will contain `type 1`, followed by node's `RGB values`

- task 3:
    - for the third task, the initial image is obtained from the compressed file
    - while reading the file, the tree is built
    - after the tree is restored, `DecompressArb` function calls a decompression algorithm that is used to create the desired image
    - images can be opened using `GIMP` or any other *Image manipulation program*


### Usage

After using `make` to compile `main.c`, there are *3 ways* to run the program, depending on the desired task:

- task 1 / 2:
    - `./quadtree [-c1 | -c2] <factor_value> <fileIn> <fileOut>`

    - run with `-c1` for *task1* or with `-c2` for *task2*
    - `factor_value` is the threshold set for the compression tree
    - `fileIn` needs to be a `.ppm` file (there are some examples in `checker/tests/input`)

- task 3:
    - `./quadtree -d <fileIn> <fileOut>`

    - `fileIn` file example: any `_c2.out` file from `checker/tests/input`
    - `fileOut` is a `.ppm` file


Images created using `task 3` can be opened using `GIMP` or any other *Image manipulation program*
