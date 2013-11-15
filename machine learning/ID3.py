import math

# calculate the entropy for options with the given probabilities
# eg. for a fair coin, entropy([0.5,0.5]) = 1
def entropy(probabilities):
    entropy = 0
    # entropy = sum[P*log2(P)]
    for P in probabilities:
        # log is undefined for non-positive numbers (so lets pretend it's zero)
        if P > 0:
            entropy = entropy - (P * math.log(P, 2))
    return entropy

# get the probabilities of each class from a list of counts of class elements
# eg. for a fair coin, probs_from_counts([1,1]) = [0.5,0.5]
def probs_from_counts(counts):
    total = sum(counts)
    return [(1.0 * count) / total for count in counts]

# a value of an attribute (= a Node in a decision tree)
class Node:
    # takes a distribution of the number of instances in each class
    # eg. 3 "yes" and 2 "no" = [3,2]
    def __init__(self, distribution = []):
        self.distribution = distribution
    # the total number of instances under this node
    def count(self):
        return sum(self.distribution)
    # get the entropy at this node
    def entropy(self):
        # return the entropy for a list of probabilities for each class
        return entropy(probs_from_counts(self.distribution))

class Attribute:
    # takes a list of distributions, each of which represents a value
    # eg. for the first value, we have 3 "yes" and 2 "no", and for the second
    # value we have 4 "yes" and 0 "no"; hence [[3,2],[4,0]]
    def __init__(self, distributions = []):
        # save the number of classes possible
        if len(distributions) > 0:
            self.num_classes = len(distributions[0])
        else:
            self.num_classes = 0
        # convert the distributions to nodes and save them
        self.nodes = [Node(distribution) for distribution in distributions]
    # get the total number of instances in all the values of this attribute
    def total(self):
        return sum([node.count() for node in self.nodes])
    # get the total number of instances in class c
    # eg. total number of "yes" according to the prior convention can be
    # gotten with self.count(0) - and number of "no" with self.count(1)
    def count(self, c):
        count = 0
        for node in self.nodes:
            count = count + node.distribution[c]
        return count
    # get the entropy before branching here
    def entropy(self):
        distribution = []
        # get the distribution of total instances for each class
        for c in range(self.num_classes):
            distribution.append(self.count(c))
        # return the entropy for a list of probabilities for each class
        return entropy(probs_from_counts(distribution))
    # get the mean information contained in this attribute
    def meaninfo(self):
        mean = 0
        total = self.total()
        # mean info = Sum[i=1->N](E[i]*P[i])
        for i in range(len(self.nodes)):
            node = self.nodes[i]
            mean = mean + (node.entropy() * node.count() / total)
        return mean
    # get the information gain from this attribute
    def infogain(self):
        return self.entropy() - self.meaninfo()
