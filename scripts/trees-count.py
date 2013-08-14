#!/usr/bin/python
import sys

# 
# count all possible trees
# get tree with given number
#

#s = 3 # terminals
#p = 5 # unary
#q = 4 # binary
#r = 3 # ternary

tc_cache = {
}

term = ['t1', 't2', 't3' ]
unar = ['u1', 'u2', 'u3', 'u4', 'u5']
binar = ['b1', 'b2', 'b3', 'b4']
ternar = ['if0', 'fold', 'fold2']


#term = ['1', '0', 'x' ]
#unar = ['not', 'shr1', 'shr16', 'shr4']
#binar = ['xor']
#ternar = []

print(term)
print(unar)
print(binar)
print(ternar)

def get2(n, m = None):
    binary = 0
    for i in range(1, (n - 2)/2): 
        j = n -1 -i
        last_binary = binary
        binary += get_trees_count(i) * get_trees_count(j)
        if m != None and m < binary:
            return [i, j, m - last_binary]

    return binary

def get3(n, m =None):
    ternary = 0
    for i in range(1, n-3):
        for j in range(1, n - 3 - i):
            k = n - 3 -i - j
            assert k > 0
            last_ternary = ternary
            ternary += get_trees_count(i) * get_trees_count(j) * get_trees_count(k)
            if m != None and m < ternary:
                return [i, j, k, m - last_ternary]
            

    return ternary

def get_trees_count(n):

    if n in tc_cache:
        return tc_cache[n]
 
    total = 0

    if n == 1:
        total = len(term)
        tc_cache[n] = total
        return total
  
    if n == 2:
        total = len(unar) * len(term)
        tc_cache[n] = total
        return total

    if n == 3 :
        total =  len(binar) * len(term) * len(term)
        tc_cache[n] = total
        return total

    
    unary = len(unar) * get_trees_count(n-1)
    binary = 0
    
    if n > 2:
        binary += get2(n)
    
    binary = binary * len(binar)

    ternary = 0

    if n > 3:
       ternary += get3(n)
 
    ternary = ternary * len(ternar)

    total = unary + binary + ternary 

    tc_cache[n] = total

    return total

def get_nth_tree(n, size):
    max_num = tc_cache[size]
    assert n >= 0
    assert n < max_num 
 
    
    if size == 1:
        return [term[n]]
 
    if size == 2:
        unary_num = n / len(term)
        terminal_num = n % len(term)
        return [unar[unary_num], get_nth_tree(terminal_num, 1)] 
    
    if size == 3:
        lt = len(term)
        binary_num = n / ( lt*lt)
        first_unary = (n % (lt*lt) ) / lt
        second_unary = (n % (lt*lt)) % lt
        return [binar[binary_num], [
                get_nth_tree(first_unary, 1), 
                get_nth_tree(second_unary, 1)
            ]
        ]
 
        
    n_3_size = tc_cache[size - 3]
    n_2_size = tc_cache[size - 2]
    n_1_size = tc_cache[size - 1]

    unary_max_num = len(unar) * n_1_size
    binary_max_num =  len(binar) * get2(size)
    ternary_max_num = len(ternar) * get3(size)

    assert unary_max_num + binary_max_num + ternary_max_num == tc_cache[size]

    if n < unary_max_num:
        # do unary
        unary = n / n_1_size
        arg = n %  n_1_size
        return [unar[unary], [get_nth_tree( arg, size - 1)]]

    elif n < (unary_max_num + binary_max_num):
        # do binary
        n2 = n - unary_max_num
        binary = n2 / get2(size)
        assert binary >= 0 
        assert binary < len(binar) 
        assert n2 < get2(size) * len(binar)
        args_n = n2 % get2(size)
        args_v = get2(size, args_n)
        arg1_max_size = tc_cache[args_v[0]] 
        arg2_max_size = tc_cache[args_v[1]]
        args_n2 = args_v[2]
        first_n  = args_n2  / arg2_max_size
        second_n = args_n2 % arg2_max_size
       # print("args_n: " + str(args_n))
       # print("args_n2: " + str(args_n2))
       # print("args_v: " + str(args_v))
       # print("arg1_max_size: " + str(arg1_max_size))
       # print("arg2_max_size: " + str(arg2_max_size))
        assert first_n < arg1_max_size, "%s < %s"%(first_n, arg1_max_size)
        assert second_n < arg2_max_size

        return [binar[binary], [
                    get_nth_tree(first_n,args_v[0]), 
                    get_nth_tree(second_n,args_v[1])
            ]
        ] 
    else:
        # do ternary
        n3 = n - unary_max_num - binary_max_num
        ternary = n3 / get3(size)
        assert ternary >=0
        assert ternary < len(ternar)
        args_n = n3 % get3(size)
        args_v = get3(size, args_n)
        arg1_max_size = tc_cache[args_v[0]]
        arg2_max_size = tc_cache[args_v[1]]
        arg3_max_size = tc_cache[args_v[2]]
        args_n3 = args_v[3]

        first_n  = args_n3 / (arg2_max_size * arg3_max_size)
        tail_n   = args_n3 % (arg2_max_size * arg3_max_size)
        second_n = tail_n / (arg3_max_size)
        third_n  = tail_n % (arg3_max_size)

        assert first_n < arg1_max_size
        assert second_n < arg2_max_size
        assert third_n < arg3_max_size

        return [ternar[ternary], [
                get_nth_tree(first_n,  args_v[0]),
                get_nth_tree(second_n, args_v[1]),
                get_nth_tree(third_n,  args_v[2])
            ]
        ]
 
        
get_trees_count(30)

print(tc_cache)

arg1 = int(sys.argv[1])
arg2 = int(sys.argv[2])

print(get_nth_tree(arg1, arg2))



