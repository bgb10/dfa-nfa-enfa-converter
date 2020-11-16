#include "Nfa.h"

Nfa::Nfa()
{
}

void Nfa::SetStates(vector<char> states)
{
	this->states = states;
}

void Nfa::SetInputSymbol(vector<char> input_symbol)
{
	this->input_symbol = input_symbol;
}

void Nfa::AddTransitionFunctionElement(char state, char input_symbol, vector<char> next_states)
{
	NfaTransitionFunctionElement element(state, input_symbol, next_states);
	transition_function.push_back(element);
}

vector<char> Nfa::TransitionFunction(char state, char symbol)
{
	//�ش� transition function�� ������ {}(empty vector) ��ȯ
	for (auto i : transition_function)
	{
		if (i.GetState() == state && i.GetInputSymbol() == symbol)
		{
			return i.GetNextStates();
		}
	}

	vector<char> empty_vector;

	return empty_vector;
}

void Nfa::SetStartState(char start_state)
{
	this->start_state = start_state;
}

void Nfa::SetFinalState(vector<char> final_state)
{
	this->final_state = final_state;
}

bool Nfa::IsFinalState(char state)
{
	if (Find(this->final_state, state) == -1)
	{
		return false;
	}

	return true;
}

int Nfa::Find(vector<char> src, char value)
{
	int result;

	result = std::find(src.begin(), src.end(), value) - src.begin(); //���� �����ϴ� ��� index ��ȯ

	if (result == src.size()) result = -1; //���� �������� �ʴ� ��� -1 ��ȯ

	return result;
}

void Nfa::Print()
{
	//�������� �����ֽ� ���� ȭ�� ��İ� ����ϰ� ����
	//States
	printf("Q (states): ");
	for (int i = 0; i < states.size(); i++)
	{
		printf("%c ", states[i]);
	}
	printf("\n");

	//Sigma
	printf("Sigma (input symbols): \n");
	for (int i = 0; i < input_symbol.size(); i++)
	{
		printf("%d) %c\n", i + 1, input_symbol[i]);
	}

	//Delta
	printf("Delta (transition function): \n");
	for (int i = 0; i < this->transition_function.size(); i++)
	{
		printf("%d) (%c, %c) = ", i + 1, transition_function[i].GetState(), transition_function[i].GetInputSymbol());
		vector<char> temp_next_states = transition_function[i].GetNextStates();
		for (int j = 0; j < temp_next_states.size(); j++)
		{
			printf("%c ", temp_next_states[j]);
		}
		printf("\n");
	}

	//q0
	printf("q0 (start state): %c", start_state);
	printf("\n");

	//F
	printf("F (final states): ");
	for (int i = 0; i < final_state.size(); i++)
	{
		printf("%d) %c", i + 1, final_state[i]);
	}
}

Dfa Nfa::Nfa2Dfa()
{
	Dfa temp_dfa;
	
	queue<vector<char>> unmarked_states;
	vector<char> T;
	vector<char> U;
	vector<char> temp_state;

	//input_symbol ����
	temp_dfa.SetInputSymbol(this->input_symbol);
	
	//start_state ����
	temp_dfa.SetStartState({ this->start_state });

	//���� ���¸� unmarked ���·� �α�
	unmarked_states.push({ start_state });

	//���� ���� �߰�
	temp_dfa.AddState({ start_state });

	//��� mark �� ������ �ݺ�
	while (!unmarked_states.empty())
	{
		T = unmarked_states.front();
		
		unmarked_states.pop();
		for (auto symbol : input_symbol)
		{
			for (auto state : T)
			{
				//TransitionFunction ����
				temp_state = TransitionFunction(state, symbol);

				//������
				vector<char> buff(U.size() + temp_state.size());
				auto iter = set_union(U.begin(), U.end(), temp_state.begin(), temp_state.end(), buff.begin());
				buff.erase(iter, buff.end());

				U = buff;
			}

			//U�� final �������� �Ǵ�
			for (auto final_candidate : U)
			{
				if (IsFinalState(final_candidate)) //U�ȿ� �ִ� ���� �� final_state�� �ִٸ�
				{
					//final state�� add�ϱ�
					temp_dfa.AddFinalState(U);
					break;
				}
			}

			//U�� Dfa�� transition function�� ���� �Ŷ��, unmarked_states�� �߰�
			if (!temp_dfa.IsStateExists(U)) 
			{
				unmarked_states.push(U); //U�� unmarked ���·� queue�� �߰�
				
				temp_dfa.AddState(U); //Dfa�� ���� �߰�
			}

			temp_dfa.AddTransitionFunction(T, symbol, U); //transition function�� ����

			U.clear();
		}
	}

	return temp_dfa;
}

void Nfa::Accept(string input)
{
	Dfa temp_dfa = this->Nfa2Dfa();

	temp_dfa.Accept(input);
}