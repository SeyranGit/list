# Implementation of a singly linked list data structure in C

```text
  _       _    ____    _____  
 | |     | |  | ___|  |_   _| 
 | |     | |  |___ \    | |   
 | |__   | |   ___) |   | |   
 |_|__|  |_|  |____/    |_|   
```

* Usage example
```c
#include <stdio.h>
#include "list/list.h"

int main(void) {
  List numbers = new_list(); 
  numbers.append(&numbers, 10);
  numbers.append(&numbers, 20);
  numbers.append(&numbers, 30);

  print_list(&numbers);
  numbers.remove(&numbers, 1);	
  print_list(&numbers);
  numbers.insert(&numbers, 0, 22);
  print_list(&numbers);
  printf("Item: %d\n", numbers.get(&numbers, 1));
  clear_list(&numbers);

  return 0;
}
```

* Output
```text
[10, 20, 30]
[10, 30]
[22, 10, 30]
Item: 10
```

### Build
___
If you don't have LLVM installed, you need to install it from the following link: [LLVM](https://github.com/llvm/llvm-project/releases/tag/llvmorg-19.1.0). Once installed, you can clone the repositories and enter the following command in the terminal if you are using Windows:
```
build
```

*Note: you may have to add the path to "../LLVM/bin" to your environment variable*
