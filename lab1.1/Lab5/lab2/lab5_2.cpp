#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstddef>
#include "cstdint"

using namespace std;

class Encoder{
private:
    vector<byte> key;
    void Key_Scheduling_Algorithm(vector<uint8_t>& S){
        for(int i = 0; i < 256; i++){
            S[i] = i;
        }
        int j = 0;
        for(int i = 0; i < 256; i++){
            j = (j + S[i] + std::to_integer<int>(key[i % key.size()])) % 256; // как сделаешь попробуй с to_integer<int>()
            swap(S[i], S[j]);
        }
    }
    void PseudoRandom_Generation_Algorithm(const vector<uint8_t>& data, vector<uint8_t>& out) {
        vector<uint8_t> S(256);
        Key_Scheduling_Algorithm(S);

        int i = 0, j = 0;
        out.resize(data.size());
        for (size_t k = 0; k < data.size(); k++) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            swap(S[i], S[j]);
            uint8_t keystream = S[(S[i] + S[j]) % 256];
            out[k] = data[k] ^ keystream;
        }
    }
public:
    explicit Encoder(const vector<byte>& key_to_init) : key(key_to_init){}

    void set_new_key(vector<byte>& new_key){
        key = new_key;
    }

    void encode(const string& input_file, const string& output_file, bool encrypt){
        ifstream inputFile(input_file, ios::binary); // обработать потенциальные ошибки
        if (!inputFile.is_open()) {
            throw runtime_error("Cannot open input file");
        }
        ofstream outputFile(output_file, ios::binary); // обработать потенциальные ошибки
        if (!outputFile.is_open()) {
            throw runtime_error("Cannot open output file");
        }
        vector<uint8_t> inputData((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();
        vector<uint8_t> outputData(inputData.size());
        PseudoRandom_Generation_Algorithm(inputData, outputData);

        outputFile.write(reinterpret_cast<const char*>(outputData.data()), outputData.size());
        outputFile.close();
    }

};

int main() {
    try {
        // Example key
        vector<byte> key = {byte('K'), byte('E'), byte('Y'), byte('!')};

        // Create encoder instance
        Encoder encoder(key);

        // Encrypt the file
        encoder.encode("input.txt", "encrypted.txt", true);

        // Decrypt the file
        encoder.encode("encrypted.txt", "decrypted.txt", false);

        cout << "Encryption and decryption completed successfully." << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}