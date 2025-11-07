#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

const double INF = 1e9;
const double EPS = 1e-9;

struct Edge {
    int from;
    int to;
    double rate;
    double weight;
    
    Edge(int f, int t, double r) : from(f), to(t), rate(r) {
        weight = -std::log(r);
    }
};

class CurrencyArbitrage {
private:
    int n;
    std::vector<Edge> edges;
    std::vector<double> dist;
    std::vector<int> prev;
    
public:
    CurrencyArbitrage(int currencies) : n(currencies) {
        dist.resize(n, INF);
        prev.resize(n, -1);
    }
    
    void addExchangeRate(int from, int to, double rate) {
        edges.emplace_back(from, to, rate);
    }
    
    std::vector<int> findArbitrageCycle() {
        dist[0] = 0;
        
        for (int i = 0; i < n; ++i) {
            for (const Edge& edge : edges) {
                if (dist[edge.from] < INF - EPS) {
                    if (dist[edge.to] > dist[edge.from] + edge.weight + EPS) {
                        dist[edge.to] = dist[edge.from] + edge.weight;
                        prev[edge.to] = edge.from;
                    }
                }
            }
        }
        
        int cycleStart = -1;
        for (const Edge& edge : edges) {
            if (dist[edge.from] < INF - EPS) {
                if (dist[edge.to] > dist[edge.from] + edge.weight + EPS) {
                    cycleStart = edge.to;
                    break;
                }
            }
        }
        
        if (cycleStart == -1) {
            return {};
        }
        
        int node = cycleStart;
        for (int i = 0; i < n; ++i) {
            node = prev[node];
        }
        
        std::vector<int> cycle;
        int startNode = node;
        do {
            cycle.push_back(node);
            node = prev[node];
        } while (node != startNode);
        
        cycle.push_back(startNode);
        std::reverse(cycle.begin(), cycle.end());
        
        return cycle;
    }
    
    double calculateProfit(const std::vector<int>& cycle) {
        if (cycle.size() < 2) return 0.0;
        
        double product = 1.0;
        for (size_t i = 0; i < cycle.size() - 1; ++i) {
            int from = cycle[i];
            int to = cycle[i + 1];
            
            for (const Edge& edge : edges) {
                if (edge.from == from && edge.to == to) {
                    product *= edge.rate;
                    break;
                }
            }
        }
        
        return product - 1.0;
    }
    
    void printArbitrageInfo(const std::vector<int>& cycle) {
        if (cycle.empty()) {
            std::cout << "No arbitration opportunity has been found." << std::endl;
            return;
        }
        
        std::cout << "An arbitration cycle has been found: ";
        for (size_t i = 0; i < cycle.size(); ++i) {
            std::cout << cycle[i];
            if (i < cycle.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
        
        double profit = calculateProfit(cycle) * 100;
        std::cout << "Profit: " << profit << "%" << std::endl;
        
        std::cout << "Details of transactions:" << std::endl;
        double amount = 1.0;
        for (size_t i = 0; i < cycle.size() - 1; ++i) {
            int from = cycle[i];
            int to = cycle[i + 1];
            double rate = 0.0;
            
            for (const Edge& edge : edges) {
                if (edge.from == from && edge.to == to) {
                    rate = edge.rate;
                    break;
                }
            }
            
            double newAmount = amount * rate;
            std::cout << "  " << amount << " currency units " << from 
                 << " -> " << newAmount << " currency units " << to 
                 << " (course: " << rate << ")" << std::endl;
            amount = newAmount;
        }
    }
};

int main() {
    CurrencyArbitrage arbitrage(4);
    
    arbitrage.addExchangeRate(0, 1, 1.2);
    arbitrage.addExchangeRate(1, 2, 0.8);
    arbitrage.addExchangeRate(2, 3, 1.5);
    arbitrage.addExchangeRate(3, 0, 0.9);
    
    std::vector<int> cycle = arbitrage.findArbitrageCycle();
    arbitrage.printArbitrageInfo(cycle);
    
    CurrencyArbitrage arbitrage2(3);
    arbitrage2.addExchangeRate(0, 1, 1.1);
    arbitrage2.addExchangeRate(1, 2, 0.9);
    arbitrage2.addExchangeRate(2, 0, 1.0);
    
    std::vector<int> cycle2 = arbitrage2.findArbitrageCycle();
    arbitrage2.printArbitrageInfo(cycle2);
    
    CurrencyArbitrage arbitrage3(4);
    arbitrage3.addExchangeRate(0, 1, 0.8);
    arbitrage3.addExchangeRate(1, 2, 1.3);
    arbitrage3.addExchangeRate(2, 3, 0.7);
    arbitrage3.addExchangeRate(3, 0, 1.4);
    
    std::vector<int> cycle3 = arbitrage3.findArbitrageCycle();
    arbitrage3.printArbitrageInfo(cycle3);
    
    return 0;
}