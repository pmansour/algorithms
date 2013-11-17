import math
from collections import defaultdict

# calculate the entropy for options with the given probabilities
# eg. for a fair coin, entropy([0.5,0.5]) = 1
def entropy(probabilities):
    entropy = 0
    # entropy = sum[P*log2(P)]
    for P in probabilities:
        # log is undefined for non-positive numbers (so lets pretend it's zero)
        if P > 0:
            entropy -= (P * math.log(P, 2))
    return entropy

# get the probabilities of each class from a list of counts of class elements
# eg. for a fair coin, probs_from_counts([1,1]) = [0.5,0.5]
def probs_from_counts(counts):
    total = sum(counts)
    return [(1.0 * count) / total for count in counts]

# a data point that has a feature (attribute) vector and a class
class Instance:
    def __init__(self, vector = {}, c = 0):
        self.vector = vector
        self.c = c
    def getclass(self):
        return self.c
    def getvalue(self, attribute):
        return self.vector[attribute]

# nodes should take the attributes so far and the data points
class Node:
    def __init__(self, attributes_remaining, instances):
        self.attributes_remaining = attributes_remaining
        self.instances = instances

        # a dictionary of value -> node for the attribute we're splitting on
        self.attribute = ''
        self.children = {}
    # the node can't be split anymore if all the instances have the same class
    def finished(self):
        # if there are no instances, then we're done
        if len(self.instances) == 0:
            return True

        # check the class of all the instances
        majority = instances[0].getclass()
        for instance in self.instances[1:]:
            # if an instance has a different class, then we're not finished
            if instance.getclass() != majority:
                return False
        # otherwise, we're done
        return True
    # get the number of instances in each classes among this node's instances
    def getcountdistribution(self, instances = None):
        # default is the distribution of all of this node's instances
        if instances == None:
            instances = self.instances
        # work out the count of each class
        counts = defaultdict(int)
        for instance in instances:
            counts[instance.getclass()] += 1
        return counts
    # get the entropy at this node
    def entropy(self, instances = None):
        # default is the entropy of all of this node's instances
        if instances == None:
            instances = self.instances
        # return the entropy for a list of probabilities for each class
        return entropy(probs_from_counts(\
                        self.getcountdistribution(instances).values()\
                    ))
    # get the majority class at this node
    def majorityclass(self):
        # get the distribution of class memberships
        distribution = self.getcountdistribution()
        # go through each all their counts to find the majority
        majorityclass = None
        majoritycount = 0
        for c in distribution.keys():
            # if the current class beats the current majority
            if distribution[c] > majoritycount:
                # replace the majority with this one
                majorityclass = c
                majoritycount = distribution[c]
        return majorityclass
    # a testing function
    def DFS(self, parents = []):
        # perform DFS on its children
        for value in self.children.keys():
            self.children[value].DFS(parents + [self.attribute + '=' + value])
        # if it's a leaf node; print its majority class and entropy
        if len(self.children) == 0:
            print ', '.join(parents)
            print 'distribution: [' +\
                ','.join(\
                        [str(c) for c in self.getcountdistribution().values()]\
                    ) +\
                    ']'
            print 'majority class: ' + self.majorityclass()
            print 'entropy: ' + str(self.entropy())

    # get the value distribution for if we split on the given attribute
    def getdistributionforattribute(self, attribute):
        # { value1 -> { class1 -> 5, class2 -> 3, ... }, ... }
        values = {}
        # for each instance in the current node
        for instance in self.instances:
            # get its value for this attribute
            value = instance.getvalue(attribute)
            # initialize this value's dictionary if needed
            if value not in values:
                values[value] = defaultdict(int)
            # increment the count of this class
            values[value][instance.getclass()] += 1
        # return the distribution we found
        return values
    # get the mean information that the given attribute would have
    def meaninfo(self, attribute):
        mean = 0
        # get the distribution of values for this attribute
        distribution = self.getdistributionforattribute(attribute)
        # the number of this node's instances
        total = len(self.instances)
        # for each value in the distribution
        for value in distribution.keys():
            # find the instances that have that value
            i = [instance for instance in self.instances \
                    if instance.getvalue(attribute) == value]
            # mean = Sum[Entropy * Probability] for all values
            E = self.entropy(i)
            P = len(i) / total
            mean += E * P
        # return the mean information
        return mean
    # perform the ID3 algorithm on this node
    def ID3(self):
        # if we're satisfied with what we have; stop
        if self.finished():
            return

        # work out the entropy at this node
        E = self.entropy()

        # initially, we don't know what the best attribute is
        best_attribute = None
        best_infogain = 0

        # for each attribute
        for attribute in self.attributes_remaining:
            # find its information gain
            IG = E - self.meaninfo(attribute)
            # if it beats what we have, keep it
            if IG > best_infogain:
                best_attribute = attribute
                best_infogain = IG

        # if we haven't found an attribute that gives an info gain; stop
        if best_attribute == None:
            return

        # since we're splitting on that attribute, take it out of the new list
        # of attributes remaining
        attributes_remaining = [att for att in self.attributes_remaining \
                                    if att != best_attribute]

        # remember the attribute we're splitting on
        self.attribute = best_attribute

        # group the instances by their value of that attribute
        # { value1 -> [instance1, instance2, ...], ... }
        values = {}
        for instance in self.instances:
            # get the value of each instance
            value = instance.getvalue(best_attribute)
            # init a list for that value if needed
            if value not in values:
                values[value] = []
            # add the instance to that value's list
            values[value].append(instance)


        # branch into child nodes
        for value in values.keys():
            # create a node for it
            valueN = Node(attributes_remaining, values[value])
            # add the new node to this node's children
            self.children[value] = valueN
            # run ID3 on it
            valueN.ID3()

# weather data
instances = [\
    Instance({ 'outlook' : 'sunny', 'temperature' : 'hot', 'humidity' : 'high', 'windy' : 'false' }, 'no'),\
    Instance({ 'outlook' : 'sunny', 'temperature' : 'hot', 'humidity' : 'high', 'windy' : 'true' }, 'no'),\
    \
    Instance({ 'outlook' : 'overcast', 'temperature' : 'hot', 'humidity' : 'high', 'windy' : 'false' }, 'yes'),\
    \
    Instance({ 'outlook' : 'rainy', 'temperature' : 'mild', 'humidity' : 'high', 'windy' : 'false' }, 'yes'),\
    Instance({ 'outlook' : 'rainy', 'temperature' : 'cool', 'humidity' : 'normal', 'windy' : 'false' }, 'yes'),\
    Instance({ 'outlook' : 'rainy', 'temperature' : 'cool', 'humidity' : 'normal', 'windy' : 'true' }, 'no'),\
    \
    Instance({ 'outlook' : 'overcast', 'temperature' : 'cool', 'humidity' : 'normal', 'windy' : 'true' }, 'yes'),\
    \
    Instance({ 'outlook' : 'sunny', 'temperature' : 'mild', 'humidity' : 'high', 'windy' : 'false' }, 'no'),\
    Instance({ 'outlook' : 'sunny', 'temperature' : 'cool', 'humidity' : 'normal', 'windy' : 'false' }, 'yes'),\
    \
    Instance({ 'outlook' : 'rainy', 'temperature' : 'mild', 'humidity' : 'normal', 'windy' : 'false' }, 'yes'),\
    \
    Instance({ 'outlook' : 'sunny', 'temperature' : 'mild', 'humidity' : 'normal', 'windy' : 'true' }, 'yes'),\
    \
    Instance({ 'outlook' : 'overcast', 'temperature' : 'mild', 'humidity' : 'high', 'windy' : 'true' }, 'yes'),\
    Instance({ 'outlook' : 'overcast', 'temperature' : 'hot', 'humidity' : 'normal', 'windy' : 'false' }, 'yes'),\
    \
    Instance({ 'outlook' : 'rainy', 'temperature' : 'mild', 'humidity' : 'high', 'windy' : 'true' }, 'no'),\
]
attributes = ['outlook', 'temperature', 'humidity', 'windy']

# test it out
tree = Node(attributes, instances)
tree.ID3()
tree.DFS()