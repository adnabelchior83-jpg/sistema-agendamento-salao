
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// 1. Estrutura de dados do Agendamento
struct Agendamento {
    int id;
    string cliente;
    string servico;
    string data;
    string horario;
};

// 2. Classe de gerenciamento do Salao de Manicure
class GerenciadorSalao {
private:
    vector<Agendamento> agendamentos;
    const string nomeArquivo = "agendamentos_salao.csv";

    // Verifica se o horario ja esta ocupado no mesmo dia
    bool horarioOcupado(const string& data, const string& horario) const {
        for (const auto& a : agendamentos) {
            if (a.data == data && a.horario == horario) {
                return true;
            }
        }
        return false;
    }

public:
    GerenciadorSalao() {
        carregarDoArquivo();
    }

    // Ordena os agendamentos por data e horario
    void ordenarAgendamentos() {
        sort(agendamentos.begin(), agendamentos.end(),
        [](const Agendamento& a, const Agendamento& b) {
            if (a.data == b.data) {
                return a.horario < b.horario;
            }
            return a.data < b.data;
        });
    }

    // Cadastra um novo agendamento
    bool cadastrarAgendamento(const string& cliente,
                              const string& servico,
                              const string& data,
                              const string& horario) {

        if (horarioOcupado(data, horario)) {
            cout << "\n[ERRO] O horario " << horario
                 << " no dia " << data
                 << " ja esta ocupado!\n";
            return false;
        }

        int novoId = 1;

        for (const auto& a : agendamentos) {
            if (a.id >= novoId) {
                novoId = a.id + 1;
            }
        }

        agendamentos.push_back({
            novoId, cliente, servico, data, horario
        });

        ordenarAgendamentos();

        cout << "\n[SUCESSO] Agendamento realizado com sucesso para "
             << cliente << "!\n";

        return true;
    }

    // Lista todos os agendamentos
    void listarAgendamentos() {
        if (agendamentos.empty()) {
            cout << "\nNenhum agendamento cadastrado.\n";
            return;
        }

        ordenarAgendamentos();

        cout << "\n=============================================================\n";
        cout << "                 AGENDA DO SALAO DE MANICURE                 \n";
        cout << "=============================================================\n";

        for (const auto& a : agendamentos) {
            cout << "Data: " << a.data
                 << " | Horario: " << a.horario
                 << " | ID: " << a.id
                 << " | Cliente: " << a.cliente
                 << " | Servico: " << a.servico << "\n";
        }

        cout << "-------------------------------------------------------------\n";
    }

    // Busca agendamentos pelo nome da cliente
    void buscarPorCliente(const string& termo) const {
        bool encontrado = false;

        cout << "\n--- RESULTADO DA BUSCA POR CLIENTE: "
             << termo << " ---\n";

        for (const auto& a : agendamentos) {
            if (a.cliente.find(termo) != string::npos) {
                cout << "ID: " << a.id
                     << " | Data: " << a.data
                     << " | Horario: " << a.horario
                     << " | Cliente: " << a.cliente
                     << " | Servico: " << a.servico << "\n";

                encontrado = true;
            }
        }

        if (!encontrado) {
            cout << "Nenhum registro encontrado.\n";
        }
    }

    // Busca agendamentos por data
    void buscarPorData(const string& dataBusca) const {
        bool encontrado = false;

        cout << "\n--- AGENDAMENTOS PARA A DATA: "
             << dataBusca << " ---\n";

        for (const auto& a : agendamentos) {
            if (a.data == dataBusca) {
                cout << "ID: " << a.id
                     << " | Horario: " << a.horario
                     << " | Cliente: " << a.cliente
                     << " | Servico: " << a.servico << "\n";

                encontrado = true;
            }
        }

        if (!encontrado) {
            cout << "Nenhum agendamento localizado para esta data.\n";
        }
    }

    // Cancela um agendamento pelo ID
    bool cancelarAgendamento(int id) {
        for (auto it = agendamentos.begin();
             it != agendamentos.end(); ++it) {

            if (it->id == id) {
                cout << "\n[SUCESSO] Agendamento ID " << id
                     << " (" << it->cliente
                     << " - " << it->data
                     << " " << it->horario
                     << ") foi cancelado!\n";

                agendamentos.erase(it);
                return true;
            }
        }

        cout << "\n[ERRO] Nenhum agendamento localizado com o ID "
             << id << ".\n";

        return false;
    }

    // Salva os dados no arquivo CSV
    void salvarEmArquivo() const {
        ofstream arquivo(nomeArquivo);

        if (!arquivo.is_open()) {
            cerr << "\n[ERRO] Nao foi possivel salvar os dados.\n";
            return;
        }

        for (const auto& a : agendamentos) {
            arquivo << a.id << ";"
                    << a.cliente << ";"
                    << a.servico << ";"
                    << a.data << ";"
                    << a.horario << "\n";
        }

        arquivo.close();

        cout << "\n[INFO] Dados salvos com sucesso em '"
             << nomeArquivo << "'.\n";
    }

    // Recupera os dados salvos no arquivo CSV
    void carregarDoArquivo() {
        ifstream arquivo(nomeArquivo);

        if (!arquivo.is_open()) {
            return;
        }

        agendamentos.clear();
        string linha;

        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;

            stringstream ss(linha);

            string idStr, cliente, servico, data, horario;

            if (getline(ss, idStr, ';') &&
                getline(ss, cliente, ';') &&
                getline(ss, servico, ';') &&
                getline(ss, data, ';') &&
                getline(ss, horario)) {

                try {
                    Agendamento a;
                    a.id = stoi(idStr);
                    a.cliente = cliente;
                    a.servico = servico;
                    a.data = data;
                    a.horario = horario;

                    agendamentos.push_back(a);
                } catch (...) {
                    cout << "\n[AVISO] Um registro invalido foi ignorado.\n";
                }
            }
        }

        arquivo.close();
        ordenarAgendamentos();
    }
};

// 3. Interface do usuario - Menu principal
int main() {

    GerenciadorSalao sistema;
    int opcao = 0;

    do {
        cout << "\n=============================================\n";
        cout << "       SISTEMA DE AGENDAMENTO                \n";
        cout << "          SALAO DE MANICURE                  \n";
        cout << "=============================================\n";

        cout << "1. Cadastrar Agendamento\n";
        cout << "2. Listar Todos os Agendamentos\n";
        cout << "3. Buscar Agendamento (Nome ou Data)\n";
        cout << "4. Cancelar Agendamento por ID\n";
        cout << "5. Salvar e Sair\n";

        cout << "\nEscolha uma opcao: ";
        cin >> opcao;
        cin.ignore(10000, '\n');

        if (opcao == 1) {

            string cliente, servico, data, horario;

            cout << "\n--- NOVO AGENDAMENTO ---\n";

            cout << "Nome da Cliente: ";
            getline(cin, cliente);

            cout << "\nServicos disponiveis como exemplo:\n";
            cout << "Manicure tradicional\n";
            cout << "Pedicure\n";
            cout << "Esmaltacao em gel\n";
            cout << "Alongamento de unhas\n";
            cout << "Manutencao de unhas\n";

            cout << "\nDigite o servico desejado: ";
            getline(cin, servico);

            cout << "Data (AAAA-MM-DD): ";
            getline(cin, data);

            cout << "Horario (HH:MM): ";
            getline(cin, horario);

            sistema.cadastrarAgendamento(
                cliente, servico, data, horario
            );

        } else if (opcao == 2) {

            sistema.listarAgendamentos();

        } else if (opcao == 3) {

            int subOpcao = 0;

            cout << "\n1. Buscar por Nome da Cliente\n";
            cout << "2. Buscar por Data (AAAA-MM-DD)\n";
            cout << "Opcao: ";

            cin >> subOpcao;
            cin.ignore(10000, '\n');

            if (subOpcao == 1) {

                string termo;

                cout << "Digite o nome ou parte do nome: ";
                getline(cin, termo);

                sistema.buscarPorCliente(termo);

            } else if (subOpcao == 2) {

                string data;

                cout << "Digite a data (AAAA-MM-DD): ";
                getline(cin, data);

                sistema.buscarPorData(data);

            } else {
                cout << "Opcao de busca invalida!\n";
            }

        } else if (opcao == 4) {

            sistema.listarAgendamentos();

            int idCancelamento;

            cout << "\nDigite o ID do agendamento a ser cancelado: ";
            cin >> idCancelamento;
            cin.ignore(10000, '\n');

            sistema.cancelarAgendamento(idCancelamento);

        } else if (opcao == 5) {

            sistema.salvarEmArquivo();

            cout << "\nEncerrando o sistema...\n";

        } else {

            cout << "\nOpcao invalida! Tente novamente.\n";
        }

    } while (opcao != 5);

    return 0;
}
