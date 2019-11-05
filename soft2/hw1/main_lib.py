import numpy as np
import copy

'''
def policy_evaluation(env, policy, gamma=0.99, theta=1e-8):
    V = np.zeros(env.nS)

    return V

def policy_improvement(env, V, gamma=0.99):
    policy = np.zeros([env.nS, env.nA]) / env.nA

    return policy
'''	

def policy_eval(env,V,policy,gamma=0.99,theta=1e-8):
	while True:
		dif=0.0
		V1=copy.deepcopy(V)
		for s in range(len(V)):
			u=0.0
			for m in range(4):
				for p in range(len(env.MDP[s][m])):
					u+=policy[s][m]*env.MDP[s][m][p][0]*(V1[env.MDP[s][m][p][1]]*gamma+env.MDP[s][m][p][2])
					V[s]=u
			dif=max(dif,abs(V[s]-V1[s]))
		if(dif<theta):
			break
	return policy_imp(env,V,policy)

def policy_imp(env,V,policy,gamma=0.99,theta=1e-8):
	stable=True
	for s in range(len(V)):
		old=copy.deepcopy(policy[s])
		max_val=0.0
		max_move=0
		for m in range(4):
			val=0.0
			for p in range(len(env.MDP[s][m])):
				val+=env.MDP[s][m][p][0]*(V[env.MDP[s][m][p][1]]*gamma+env.MDP[s][m][p][2])
			if val>max_val:
				max_val=val
				max_move=m
		for k in range(4):
			policy[s][k]=0.0
		policy[s][max_move]=1.0
		if np.array_equal(old,policy[s])==False:
			stable=False
	if stable==True:
		return policy, V
	else:
		return policy_eval(env,V,policy)

def value_eval(env,V,policy,gamma=0.99,theta=1e-8):
	while True:
		dif=0.0
		V1=copy.deepcopy(V)
		for s in range(len(V)):
			max_val=0.0
			max_move=0
			for m in range(4):
				val=0.0
				for p in range(len(env.MDP[s][m])):
					val+=env.MDP[s][m][p][0]*(V1[env.MDP[s][m][p][1]]*gamma+env.MDP[s][m][p][2])
				if(val>max_val):
					max_val=val
					max_move=m
			V[s]=max_val	
			dif=max(dif,abs(V[s]-V1[s]))
		if(dif<theta):
			break

	for s in range(len(V)):
		max_val=0.0
		max_move=0
		for m in range(4):
			val=0.0
			for p in range(len(env.MDP[s][m])):
				val+=env.MDP[s][m][p][0]*(V[env.MDP[s][m][p][1]]*gamma+env.MDP[s][m][p][2])
			if val>max_val:
				max_val=val
				max_move=m
		for k in range(4):
			policy[s][k]=0.0
		policy[s][max_move]=1.0

	return policy, V

def policy_iteration(env, gamma=0.99, theta=1e-8):
    V = np.zeros(env.nS)
    policy = np.ones([env.nS, env.nA]) / env.nA
    return policy_eval(env,V,policy)

def value_iteration(env, gamma=0.99, theta=1e-8):
    V = np.zeros(env.nS)
    policy = np.ones([env.nS, env.nA]) / env.nA

    return value_eval(env,V,policy)
