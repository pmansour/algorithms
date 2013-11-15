import math

def entropy(probabilities):
    entropy = 0
    # entropy = Sum[P*log2(P)]
    for P in probabilities:
        # log is not defined for non-positive numbers (pretend it's zero)
        if P > 0:
            entropy = entropy - (P * math.log(P, 2))
    return entropy

class Node:
    def __init__(self, classes = []):
        self.classcounts = classes
    # add a new class with the given number of instances
    def addclass(self, count):
        self.classcounts.append(count)
    # the total number of instances under this node
    def count(self):
        return sum(self.classcounts)
    # get the entropy at this node
    def entropy(self):
        # get the sum of all the counts
        total = self.count()
        return entropy([((1.0 * count) / total) for count in self.classcounts])
class Attribute:
    # takes a list of lists, each of which is a distribution of a value
    def __init__(self, nodes = []):
        self.nodes = [Node(classes) for classes in nodes]
    # add a new node with the given class counts
    def addnode(self, classes = []):
        self.nodes.append(Node(classes))
    # get the total number of instances in all the values of this attribute
    def count(self):
        return sum([node.count() for node in self.nodes])
    # get the entropy before branching here
    def entropy(self):
        classcounts = []
        total = self.count()
        first = True
        for node in self.nodes:
            for i in range(len(node.classcounts)):
                if first:
                    prior = 0
                else:
                    prior = classcounts[i]
                classcounts[i] = prior + node.classcounts[i]
            first = False
        return entropy([((1.0 * count) / total) for count in classcounts])
    # get the mean information in this attribute
    def meaninfo(self):
        mean = 0
        total = self.count()
        # mean info = Sum[i=1->N](E[i]*P[i])
        for i in range(len(self.nodes)):
            node = self.nodes[i]
            mean = mean + (node.entropy() * node.count() / total)
        return mean
