#include "eNfa.h"

eNfa::eNfa()
{
}

void eNfa::SetStates(vector<char> states)
{
	this->states = states;
}

void eNfa::SetInputSymbol(vector<char> input_symbol)
{
	this->input_symbol = input_symbol;
	//this->input_symbol.push_back('#'); //epsilon �߰�
}

void eNfa::AddTransitionFunctionElement(char state, char input_symbol, vector<char> next_states)
{
	eNfaTransitionFunctionElement element(state, input_symbol, next_states);
	this->transition_function.push_back(element);
}

vector<char> eNfa::TransitionFunction(char state, char symbol)
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

void eNfa::SetStartState(char start_state)
{
	this->start_state = start_state;
}

void eNfa::SetFinalState(vector<char> final_state)
{
	this->final_state = final_state;
}

bool eNfa::IsFinalState(char state)
{
	if (Find(this->final_state, state) == -1)
	{
		return false;
	}

	return true;
}

int eNfa::Find(vector<char> src, char value)
{
	int result;

	result = std::find(src.begin(), src.end(), value) - src.begin(); //���� �����ϴ� ��� index ��ȯ

	if (result == src.size()) result = -1; //���� �������� �ʴ� ��� -1 ��ȯ

	return result;
}

Dfa eNfa::eNfa2Dfa()
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
	unmarked_states.push(CL({ this->start_state }));

	//���� ���� �߰�
	temp_dfa.AddState(CL({ this->start_state }));

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

			U = CL(U);
			sort(U.begin(), U.end());
			if (U.empty())
			{
				U.clear();
				continue;
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

			sort(T.begin(), T.end());
			temp_dfa.AddTransitionFunction(T, symbol, U); //transition function�� ����

			U.clear();
		}
	}

	return temp_dfa;
}

vector<char> eNfa::CL(vector<char> state)
{
	queue<char> CL_state_queue;
	vector<char> CL_state;
	vector<char> temp_next_state;
	char temp_state;

	for (auto state_element : state)
	{
		CL_state_queue.push(state_element);
	}

	while (!CL_state_queue.empty())
	{
		temp_state = CL_state_queue.front();

		if (Find(CL_state, temp_state) == -1) //vector�� ���ٸ�
		{
			//vector�� �߰�
			CL_state.push_back(temp_state);

			//epsilon�� ���� �� �� �ִ� next_state���� queue�� �߰�
			temp_next_state = TransitionFunction(temp_state, '#');

			for (auto next_state : temp_next_state)
			{
				CL_state_queue.push(next_state);
			}
		}

		CL_state_queue.pop();
	}

	return CL_state;
}

void eNfa::Print()
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

void eNfa::Accept(string input)
{
	Dfa temp_dfa = this->eNfa2Dfa();

	temp_dfa.Accept(input);
}