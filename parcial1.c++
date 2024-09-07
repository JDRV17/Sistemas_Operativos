#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> 
#include <numeric>

using namespace std;

void FCFS(vector<string>& gantt, const vector<string>& pids, vector<int>& bts, int& time, int index)
{
    for(int i=0; i<bts[index]; i++)
    {
        gantt[time] = pids[index];
        time += 1;
    }
    bts[index] = 0;
}

void RR(vector<string>& gantt, const vector<string>& pids, vector<int>& bts, int& time, int index, int quantum)
{
    int i = 0;
    while (i < quantum && bts[index] > 0)
    {
        gantt[time] = pids[index];
        time += 1;
        bts[index] -= 1;
        i += 1;
    }
}

void mlq()
{
    ifstream file ("procesos_mlq.txt");
    string line, pid, atstr, btstr, qstr;
    int at, bt, q, index, quantum, total, time;
    double ct, rt, wt, tat;
    time = 0;
    vector<string> pids, gantt;
    vector<int> ats, bts, qs, cts, rts, wts, tats, btsorg;
    vector<bool> ready, started;
    while (getline(file, line))
    {
        istringstream iss(line);
        iss >> pid >> atstr >> btstr >> qstr;
        at = stoi(atstr);
        bt = stoi(btstr);
        q = stoi(qstr);
        pids.push_back(pid);
        ats.push_back(at);
        bts.push_back(bt);
        btsorg.push_back(bt);
        qs.push_back(q);
    }
    total = accumulate(bts.begin(), bts.end(), 0);
    gantt.resize(total); 
    ready.resize(ats.size(), false);
    started.resize(ats.size(), false);
    cts.resize(ats.size(), 0);
    auto min = min_element(ats.begin(), ats.end());
    index = distance(ats.begin(), min);
    while(gantt[total-1] == "")
    {
        if (ats[index] <= time)
        {
            ready[index] = true;
        }
        if (ready[index])
        {
            if (started[index] == false)
            {
                rts.push_back(time);
            }
            started[index] = true;
            if(qs[index] == 4 && ats[index] <= time)
            {
                if (bts[index] != 0)
                {
                    cts[index] = time + bts[index];
                }
                FCFS(gantt, pids, bts, time, index);
            }
            else 
            {
                quantum = 1 << (qs[index] - 1);
                RR(gantt, pids, bts, time, index, quantum);
                if (bts[index] == 0)
                {
                    cts[index] = time;
                    bts[index] -= 1;
                }
            }
        }  
        if (index == ats.size()-1)
        {
            index = 0;
        }
        else
        {
            index += 1;
        }
    }
    for(int i=0; i<ats.size(); i++)
    {
        tats.push_back(cts[i] - ats[i]);
    }
    for(int i=0; i<ats.size(); i++)
    {
        wts.push_back(cts[i] - ats[i] - btsorg[i]);
    }
    ct = static_cast<double>(accumulate(cts.begin(), cts.end(), 0))/cts.size();
    rt = static_cast<double>(accumulate(rts.begin(), rts.end(), 0))/rts.size();
    wt = static_cast<double>(accumulate(wts.begin(), wts.end(), 0))/wts.size();
    tat = static_cast<double>(accumulate(tats.begin(), tats.end(), 0))/tats.size();
    cout << "El diagrama de Gantt se ve así: " << endl;
    for (int i = 0; i<total; i++)
    {
        cout << gantt[i];
    }
    
    cout << endl << "Los tiempos de completación (CT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << cts[i] << endl;
    }
    cout << "Para un CT promedio de: " << ct << endl;
    cout << "Los tiempos de respuesta (RT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << rts[i] << endl;
    }
    cout << "Para un RT promedio de: " << rt << endl;
    cout << "Los tiempos de espera (WT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << wts[i] << endl;
    }
    cout << "Para un WT promedio de: " << wt << endl;
    cout << "Los tiempos turn-around (TAT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << tats[i] << endl;
    }
    cout << "Para un TAT promedio de: " << tat << endl;
    if (!file.is_open())
    {
        cerr << "No se pudo abrir el archivo." << endl;
    }
    file.close();
}

void mlfq()
{
    ifstream file ("procesos_mlq.txt");
    string line, pid, atstr, btstr;
    int at, bt, index, quantum, total, time;
    double ct, rt, wt, tat;
    time = 0;
    vector<string> pids, gantt;
    vector<int> ats, bts, qs, cts, rts, wts, tats, btsorg;
    vector<bool> ready, started;
    while (getline(file, line))
    {
        istringstream iss(line);
        iss >> pid >> atstr >> btstr;
        at = stoi(atstr);
        bt = stoi(btstr);
        pids.push_back(pid);
        ats.push_back(at);
        bts.push_back(bt);
        btsorg.push_back(bt);
        qs.push_back(1);
    }
    total = accumulate(bts.begin(), bts.end(), 0);
    gantt.resize(total); 
    ready.resize(ats.size(), false);
    started.resize(ats.size(), false);
    cts.resize(ats.size(), 0);
    auto min = min_element(ats.begin(), ats.end());
    index = distance(ats.begin(), min);
    while(gantt[total-1] == "")
    {
        if (ats[index] <= time)
        {
            ready[index] = true;
        }
        if (ready[index])
        {
            if (started[index] == false)
            {
                rts.push_back(time);
            }
            started[index] = true;
            if(qs[index] == 4 && ats[index] <= time)
            {
                if (bts[index] != 0)
                {
                    cts[index] = time + bts[index];
                }
                FCFS(gantt, pids, bts, time, index);
            }
            else 
            {
                quantum = 1 << (qs[index] - 1);
                RR(gantt, pids, bts, time, index, quantum);
                if (bts[index] == 0)
                {
                    cts[index] = time;
                    bts[index] -= 1;
                }
                else if (qs[index] < 3)
                {
                    qs[index] += 1;
                }
            }
        }  
        if (index == ats.size()-1)
        {
            index = 0;
        }
        else
        {
            index += 1;
        }
    }
    for(int i=0; i<ats.size(); i++)
    {
        tats.push_back(cts[i] - ats[i]);
    }
    for(int i=0; i<ats.size(); i++)
    {
        wts.push_back(cts[i] - ats[i] - btsorg[i]);
    }
    ct = static_cast<double>(accumulate(cts.begin(), cts.end(), 0))/cts.size();
    rt = static_cast<double>(accumulate(rts.begin(), rts.end(), 0))/rts.size();
    wt = static_cast<double>(accumulate(wts.begin(), wts.end(), 0))/wts.size();
    tat = static_cast<double>(accumulate(tats.begin(), tats.end(), 0))/tats.size();
    cout << "El diagrama de Gantt se ve así: " << endl;
    for (int i = 0; i<total; i++)
    {
        cout << gantt[i];
    }
    
    cout << endl << "Los tiempos de completación (CT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << cts[i] << endl;
    }
    cout << "Para un CT promedio de: " << ct << endl;
    cout << "Los tiempos de respuesta (RT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << rts[i] << endl;
    }
    cout << "Para un RT promedio de: " << rt << endl;
    cout << "Los tiempos de espera (WT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << wts[i] << endl;
    }
    cout << "Para un WT promedio de: " << wt << endl;
    cout << "Los tiempos turn-around (TAT) son " << endl;
    for (int i = 0; i<ats.size(); i++)
    {
        cout << "P" << i << ": " << tats[i] << endl;
    }
    cout << "Para un TAT promedio de: " << tat << endl;
    if (!file.is_open())
    {
        cerr << "No se pudo abrir el archivo." << endl;
    }
    file.close();
}

int main()
{
    cout << "Para el caso de MLQ, se asumirá que la cola 1 se planifica con RR(1), la cola 2 con RR(2), la cola 3 con RR(4) y la cola 4 con FCFS" << endl;
    mlq();
    cout << "Para el caso de MLFQ, se asumirá que la cola 1 se planifica con RR(1), la cola 2 con RR(2), la cola 3 con RR(4) y la cola 4 con FCFS" << endl;
    mlfq();

    return 0;
}