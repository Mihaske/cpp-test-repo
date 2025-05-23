#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>
using namespace std;
const int MAX_RESULT_DOCUMENT_COUNT = 5;
string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};
class SearchServer {
public:


    void SetStopWords(const string &text) {
        for (const string &word: SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string &document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        for (const string &word: words) {
            word_to_documents_freqs_[word][document_id] += 1.0 / words.size();
        //важно написать именно 1.0 / иначе посчитает неверно
        }
        ++allk;
    }

    vector<Document> FindTopDocuments(const string &raw_query) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document &lhs, const Document &rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:

    struct Query {
        set<string> plus_;
        set<string> minus_;
    };

    map<string, map<int, double>> word_to_documents_freqs_;

    set<string> stop_words_;

    int allk = 0; // при помощи AddDocument будет хранить кол-во документов

    bool IsStopWord(const string &word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string &text) const {
        vector<string> words;
        for (const string &word: SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string &text) const {
        Query query;
        for (const string &word: SplitIntoWordsNoStop(text)) {
            if (word[0] == '-') {
                query.minus_.insert(word.substr(1));
             } else {
                query.plus_.insert(word);
                }
        }
        return query;
    }
       vector<Document> FindAllDocuments(const Query &query_words) const {
        vector<Document> matched_documents;
         double idf;
         map<int,double> infid;
        map<string, map<int, double>> slova;
        for ( const string& qw : query_words.plus_){
             if (query_words.minus_.count(qw)>0)
                 continue;
                if (word_to_documents_freqs_.count(qw)==1){
                      idf = log(1.0 * allk /  word_to_documents_freqs_.at(qw).size());
        for (const auto& [id, tid] :  word_to_documents_freqs_.at(qw)){
                    //aналогично обязательно (наверно) писать 1.0
                     slova[qw][id] = tid * idf;

                 }}}
    map<int,double>md;
         for (const auto& [sl,iid] : slova){
             for (const auto &[id, r] : iid){
                         md[id]+=r;
             } }
         for (const auto& [id,relevance] : md){
        matched_documents.push_back({id, relevance});}
        return matched_documents;
   }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
