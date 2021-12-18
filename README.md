# PriorityQueue
A simple implementation of a min-heap priority queue for [io](https://iolanguage.org/) made to help solve the [Advent of Code](https://github.com/lewis-weinberger/aoc.git) problems.

## Installation
This addon can be installed using the [eerie](https://github.com/IoLanguage/eerie) package manager:

```sh
eerie install https://github.com/lewis-weinberger/PriorityQueue.git
```

## Usage

Create a new queue with a pre-allocated heap using `with`:

```io
queue := PriorityQueue with(100) # capacity for 100 elements before internal reallocation
```

Add a new value to the queue with integer priority using `push`:

```io
value := 10
priority := 1
queue push(value, priority)
```

Remove and return the next element in the queue with `pop`:

```io
highestPriority := queue pop
```

Note as this is a [min-heap](https://en.wikipedia.org/wiki/Binary_heap), the "highest priority" element next in the queue has the lowest priority value. 

Finally you can query the size of the queue with `size`:

```io
remaining := queue size
```

A longer example:

```io
queue := PriorityQueue with(5)

queue push(5, 1)
queue push("hello", 4)
queue push(list(4, 4), 3)
queue push(3, 2)
queue push(2, 5)

queue size # 5

queue pop # 5
queue pop # 3
queue pop # list(4, 4)
queue pop # "hello"
queue pop # 2

queue pop # returns nil when empty
```

## License

[MIT](./LICENSE)
