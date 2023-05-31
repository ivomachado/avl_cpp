# avl_cpp

When I was majoring Computer Science the only algorithm that I couldn't finish during my studies at the time was the AVL tree remotion. 5 years later I decided to give it another spin and it finally came out.

I tried to apply DRY extensively and played a lot with templates and `<type_traits>` with a goal to practice more advanced C++ features while implementing the AVL.

The bulk of the implementation lies in the `avl::Node` class located at `src/avl/Node.h`, the `avl::Tree` being merely a wrapper for ergonomics during usage.


## The Tree

The AVL Tree is a binary search tree with **strict balance** and every node has nodes with bigger keys to the right and smaller to the left.
It has three operations: Insertion, Search, and Remotion.

Every node has a **balance factor** with values in {-1, 0, 1}.
The factor is the difference between the heights from the right and left subtrees.
This means that no node has subtrees with their height difference bigger than 1.

The three has to rebalance during insertions and remotions by performing rotations based on nodes' balance factors.

### Rotations

Rotations change the relations between three nodes to fix a node with a balance factor of 2 or -2.
The type of rotation is performed based on the subchildren's balance factors, the `avl::Node::rotate` method just receives the direction to rotate and is able to make the proper rotation type.

#### Simple Rotation to Left

After an operation that a node has a 2 balance factor and its right child has 1 or 0,
a simple rotation to left is required to regain balance.
The node becomes the left child of its former right child and their subtrees are rearranged according to the picture bellow:
![image](https://github.com/ivomachado/avl_cpp/assets/9119113/94cffd2f-7548-492b-a723-074fd4679915)


The two possibilities for the right child's balance factor are in orange and green in the picture along with the resulting balance factors.

I implemented this in the `avl::Node::rotateSimple` method.


#### Simple Rotation to Right

The principle is the same as rotation to left, but the balance factors are negative instead of positive. I implemented in the same `avl::Node::rotateSimple` method, just changing the `direction` parameter.

#### Double Rotation to Right

When an operation finishes with a node that has a -2 for balance factor and its left child has -1, it becomes necessary to make a double rotation, first to left then to right.

The first rotation is to the left and happens first between the node's left child and its right child. The left child becomes the left subtree of its former right child and this becomes the new rebalancing node's left child. This leaves the tree ready for a simple rotation to the right that completes the rotation as shown in the picture bellow:
![image](https://github.com/ivomachado/avl_cpp/assets/9119113/a45c1d8f-dee6-4b1c-a00e-d8abc0ff3024)

There are three possibilities for resulting balance factors and they have the same text colors as their origin and result.

#### Double Rotation to Left

The principle is the same as rotation to right, but the balance factors are multiplied by -1. I implemented in the same `avl::Node::rotateDouble` method, just changing the `direction` parameter.

### Search

The search is done the same as any binary search three, recursively see if the current node is equal to the searched key or where to continue the search until the node is found or the search is exausted. I didn't bother to implement it because my real challange was the remotion.

### Insertion

The `avl::Node::insert` is implemented rescursively with a base case that a node is smaller or bigger than the inserted key but no subtree in the direction that this new insertion should be, so this node receives the newly created node. After that, every recursive call checks if the node bellow that finished its insertion subcall has got bigger.

If any node realizes that its child got taller, it updates the balance factor and check if a rebalance is needed. If only a simple or double rotation is required to regain balance without compromising the nodes above.

### Remotion

I implemented the remotion in `avl::Node::remove` method. It receives a pointer to the current node and the searched key, returning the replacement child to the level above, along with the found value.

The remotion is challenging to implement because it needs to keep the tree balanced after the operation when there are multiple cases where the removed node can be positioned in the tree.

The method calls itself recursively for the child in the search direction until it finds the key. Following the recursive call, it assigns the returned node as the node's child, updates its balance factor then rebalances itself based on its children's balance. I'm talking about the cases first, then about the rebalance itself.

#### Leaf Remotion

The leaf case is the easiest: return null to its father to complete its removal from the tree. With only one child, the implementation returns this single subtree.

#### Node with Single Child

With only one child, the implementation returns this single subtree.

#### Node with Two Children

The most complex case is when both children are present. I implemented that the left child always takes its father's place, but this requires a left child's sub-node to take its father's position. The left child is replaced by its right-most descendant, bringing its replacement left subtree one level above.

#### Rebalancing

After every recursive return call, the node needs to compare its children balance factorss to theirs previous values to update its balance factor and then check if a rebalance is needed.

Different from the insertion, every node needs to check if it got unbalanced and then rebalance with one the rotations. This was one the greatest challanges that I faced while implementing this tree.

## Building

```bash
# At project root
mkdir build && cd build
cmake ..
cmake --build .
```

## Running
There is a pretty simple executable called `avl_cpp` that creates a hardcoded tree with a few insertions and displays it. The fun part lies in the `avl_cpp_test` that I wrote before the implementation itself with Test-Driven Development in mind and a few stress tests.
