# avl_cpp

When I was coursing Computer Science the only algorithm that I couldn't finish during my studies at the time was the AVL tree remotion. 5 years later I decided to give it another spin and it finally came out.

I tried to apply DRY extensively and played a lot with templates and `<type_traits>` to achieve this, my goal was to practice more intermediate C++ functionalities while implementing the AVL.

The bulk of the implementation lies in the `avl::Node` class located at `src/avl/Node.h`, the `avl::Tree` being merely a wrapper for ergonomics during usage.
## The Tree

The AVL Tree is a binary search tree with **strict balancing**.
It has three operations: Insertion, Search, and Remotion.

Every node has a **balance factor** with values in {-1, 0, 1}.
The factor is the difference between the heights from the right and left subtrees.

The three rebalances during insertions and remotions by performing rotations based on nodes' balance factors.

### Rotations

Rotations change the relations between three nodes to fix a node with a balance factor of 2 or -2.
The type of rotation is performed based on the subchildren's balance factors and which subtree is unbalanced.

#### Simple Rotation to Left

After an operation that a node has a 2 balance factor and its right child has 1 or 0,
a simple rotation to left is required to regain balancing.
The node becomes the left child of its former right child and their subtrees are rearranged according to the picture bellow:
![image](https://github.com/ivomachado/avl_cpp/assets/9119113/618d8172-0702-4dcc-a31f-af08dcebb560)

The two possibilities for the right child's balance factor are in orange and green in the picture along with the resulting balance factors.

I implemented this in the `avl::Node::rotateSimple` method.


#### Simple Rotation to Right

The principle is the same as rotation to left, but the balance factors are negative instead of positive. I implemented in the same `avl::Node::rotateSimple` method, just changing the `direction` parameter.

#### Double Rotation to Right

TBD

#### Double Rotation to Left

TBD

### Search

TBD

### Insertion

TBD

### Remotion

I implemented the remotion in `avl::Node::remove` method. It receives a pointer to the current node and the searched key, returning the replacement child to the level above, along with the found value.

The remotion is challenging to implement because it needs to keep the tree balanced after the operation when there are multiple cases where the removed node can be positioned in the tree.

The method calls itself recursively for the child in the search direction until it finds the key. Following the recursive call, it assigns the returned node as the node's child, updates its balance factor then rebalances itself based on its children's balancing. I'm talking about the cases first, then about the rebalance itself.

#### Leaf Remotion

The leaf case is the easiest: return null to its father to complete its removal from the tree. With only one child, the implementation returns this single subtree.

#### Node with Single Child

With only one child, the implementation returns this single subtree.

#### Node with Two Children

The most complex case is when both children are present. I implemented that the left child always takes its father's place, but this requires a left child's sub-node to take its father's position. The left child is replaced by its right-most descendant, bringing its replacement left subtree one level above.


## Building

```bash
# At project root
mkdir build && cd build
cmake ..
make
```

## Running
There is a pretty simple executable called `avl_cpp` that creates a hardcoded tree with a few insertions and displays it. The fun part lies in the `avl_cpp_test` that I wrote before the implementation itself with Test-Driven Development in mind and a few stress tests.
