#include <iostream>
#include <string>
#include <vector>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

class Dataset_object {
    // the dataset_object is the data in each cell in the dataset
    // it can be a string, int, float, or bool
public:
    int type;
    string sval = "";
    int ival;
    float fval;
    bool bval;
    Dataset_object(string sval) {
        this->sval = sval;
        type = 1;
    }
    Dataset_object(int ival) {
        this->ival = ival;
        type = 2;
    }
    Dataset_object(float fval) {
        this->fval = fval;
        type = 3;
    }
    Dataset_object(bool bval) {
        this->bval = bval;
        type = 4;
    }
}

class Dataset {
    // the dataset is a 2D array of dataset_objects
public:
    vector<vector<Dataset_object>> data;
    Dataset(vector<vector<Dataset_object>> data) {
        this->data = data;
    }

    // operator overloading for the + operator only if the two datasets have the same shape and same type in corresponding cells
    Dataset operator+(Dataset &other) {
        if (this->data.size() != other.data.size()) {
            cout << "Error: Datasets have different shapes" << endl;
            return Dataset({});
        }
        for (int i = 0; i < this->data.size(); i++) {
            if (this->data[i].size() != other.data[i].size()) {
                cout << "Error: Datasets have different shapes" << endl;
                return Dataset({});
            }
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type != other.data[i][j].type) {
                    cout << "Error: Datasets have different types" << endl;
                    return Dataset({});
                }
            }
        }
        vector<vector<Dataset_object>> new_data;
        for (int i = 0; i < this->data.size(); i++) {
            vector<Dataset_object> row;
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type == 1) {
                    row.push_back(Dataset_object(this->data[i][j].sval + other.data[i][j].sval));
                } else if (this->data[i][j].type == 2) {
                    row.push_back(Dataset_object(this->data[i][j].ival + other.data[i][j].ival));
                } else if (this->data[i][j].type == 3) {
                    row.push_back(Dataset_object(this->data[i][j].fval + other.data[i][j].fval));
                } else if (this->data[i][j].type == 4) {
                    row.push_back(Dataset_object(this->data[i][j].bval + other.data[i][j].bval));
                }
            }
            new_data.push_back(row);
        }
        return Dataset(new_data);
    }

    // operator overloading for the - operator only if the two datasets have the same shape and same type in corresponding cells
    Dataset operator-(Dataset &other) {
        if (this->data.size() != other.data.size()) {
            cout << "Error: Datasets have different shapes" << endl;
            return Dataset({});
        }
        for (int i = 0; i < this->data.size(); i++) {
            if (this->data[i].size() != other.data[i].size()) {
                cout << "Error: Datasets have different shapes" << endl;
                return Dataset({});
            }
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type != other.data[i][j].type) {
                    cout << "Error: Datasets have different types" << endl;
                    return Dataset({});
                }
            }
        }
        vector<vector<Dataset_object>> new_data;
        for (int i = 0; i < this->data.size(); i++) {
            vector<Dataset_object> row;
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type == 1) {
                    row.push_back(Dataset_object(this->data[i][j].sval - other.data[i][j].sval));
                } else if (this->data[i][j].type == 2) {
                    row.push_back(Dataset_object(this->data[i][j].ival - other.data[i][j].ival));
                } else if (this->data[i][j].type == 3) {
                    row.push_back(Dataset_object(this->data[i][j].fval - other.data[i][j].fval));
                } else if (this->data[i][j].type == 4) {
                    row.push_back(Dataset_object(this->data[i][j].bval - other.data[i][j].bval));
                }
            }
            new_data.push_back(row);
        }
        return Dataset(new_data);
    }

    // operator overloading for the * operator only if the two datasets have the same shape and same type in corresponding cells
    Dataset operator*(Dataset &other) {
        if (this->data.size() != other.data.size()) {
            cout << "Error: Datasets have different shapes" << endl;
            return Dataset({});
        }
        for (int i = 0; i < this->data.size(); i++) {
            if (this->data[i].size() != other.data[i].size()) {
                cout << "Error: Datasets have different shapes" << endl;
                return Dataset({});
            }
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type != other.data[i][j].type) {
                    cout << "Error: Datasets have different types" << endl;
                    return Dataset({});
                }
            }
        }
        vector<vector<Dataset_object>> new_data;
        for (int i = 0; i < this->data.size(); i++) {
            vector<Dataset_object> row;
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type == 1) {
                    row.push_back(Dataset_object(this->data[i][j].sval * other.data[i][j].sval));
                } else if (this->data[i][j].type == 2) {
                    row.push_back(Dataset_object(this->data[i][j].ival * other.data[i][j].ival));
                } else if (this->data[i][j].type == 3) {
                    row.push_back(Dataset_object(this->data[i][j].fval * other.data[i][j].fval));
                } else if (this->data[i][j].type == 4) {
                    row.push_back(Dataset_object(this->data[i][j].bval * other.data[i][j].bval));
                }
            }
            new_data.push_back(row);
        }
        return Dataset(new_data);
    }

    // operator overloading for the / operator only if the two datasets have the same shape and same type in corresponding cells
    Dataset operator/(Dataset &other) {
        if (this->data.size() != other.data.size()) {
            cout << "Error: Datasets have different shapes" << endl;
            return Dataset({});
        }
        for (int i = 0; i < this->data.size(); i++) {
            if (this->data[i].size() != other.data[i].size()) {
                cout << "Error: Datasets have different shapes" << endl;
                return Dataset({});
            }
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type != other.data[i][j].type) {
                    cout << "Error: Datasets have different types" << endl;
                    return Dataset({});
                }
            }
        }
        vector<vector<Dataset_object>> new_data;
        for (int i = 0; i < this->data.size(); i++) {
            vector<Dataset_object> row;
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type == 1) {
                    row.push_back(Dataset_object(this->data[i][j].sval / other.data[i][j].sval));
                } else if (this->data[i][j].type == 2) {
                    row.push_back(Dataset_object(this->data[i][j].ival / other.data[i][j].ival));
                } else if (this->data[i][j].type == 3) {
                    row.push_back(Dataset_object(this->data[i][j].fval / other.data[i][j].fval));
                } else if (this->data[i][j].type == 4) {
                    row.push_back(Dataset_object(this->data[i][j].bval / other.data[i][j].bval));
                }
            }
            new_data.push_back(row);
        }
        return Dataset(new_data);
    }

    // operator overloading for the = operator only if the two datasets have the same shape and same type in corresponding cells
    Dataset operator=(Dataset &other) {
        if (this->data.size() != other.data.size()) {
            cout << "Error: Datasets have different shapes" << endl;
            return Dataset({});
        }
        for (int i = 0; i < this->data.size(); i++) {
            if (this->data[i].size() != other.data[i].size()) {
                cout << "Error: Datasets have different shapes" << endl;
                return Dataset({});
            }
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type != other.data[i][j].type) {
                    cout << "Error: Datasets have different types" << endl;
                    return Dataset({});
                }
            }
        }
        vector<vector<Dataset_object>> new_data;
        for (int i = 0; i < this->data.size(); i++) {
            vector<Dataset_object> row;
            for (int j = 0; j < this->data[i].size(); j++) {
                if (this->data[i][j].type == 1) {
                    row.push_back(Dataset_object(other.data[i][j].sval));
                } else if (this->data[i][j].type == 2) {
                    row.push_back(Dataset_object(other.data[i][j].ival));
                } else if (this->data[i][j].type == 3) {
                    row.push_back(Dataset_object(other.data[i][j].fval));
                } else if (this->data[i][j].type == 4) {
                    row.push_back(Dataset_object(other.data[i][j].bval));
                }
            }
            new_data.push_back(row);
        }
        return Dataset(new_data);
    }
}

vector<Dataset_object> func_row(Dataset dataset, int row) {
    return dataset.data[row];
}

Dataset func_row(Dataset dataset, vector<int> rows) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < rows.size(); i++) {
        new_data.push_back(dataset.data[rows[i]]);
    }
    return Dataset(new_data);
}

vector<Dataset_object> func_row(Dataset dataset, string s){
    vector<Dataset_object> row;
    for (int i = 0; i < dataset.data.size(); i++) {
        if (dataset.data[i][0].sval == s) {
            row = dataset.data[i];
            break;
        }
    }
    return row;
}

Dataset func_row(Dataset dataset, vector<string> rows) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < rows.size(); i++) {
        new_data.push_back(func_row(dataset, rows[i]));
    }
    return Dataset(new_data);
}

Dataset func_row(Dataset dataset, vector<vector<int>> rows) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < rows.size(); i++) {
        new_data.push_back(func_row(dataset, rows[i]));
    }
    return Dataset(new_data);
}

vector<Dataset_object> func_col(Dataset dataset, int col) {
    vector<Dataset_object> col_data;
    for (int i = 0; i < dataset.data.size(); i++) {
        col_data.push_back(dataset.data[i][col]);
    }
    return col_data;
}

Dataset func_col(Dataset dataset, vector<int> cols) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < dataset.data.size(); i++) {
        vector<Dataset_object> row;
        for (int j = 0; j < cols.size(); j++) {
            row.push_back(dataset.data[i][cols[j]]);
        }
        new_data.push_back(row);
    }
    return Dataset(new_data);
}

vector<Dataset_object> func_col(Dataset dataset, string s) {
    vector<Dataset_object> col;
    for (int i = 0; i < dataset.data[0].size(); i++) {
        if (dataset.data[0][i].sval == s) {
            col = func_col(dataset, i);
            break;
        }
    }
    return col;
}

Dataset func_col(Dataset dataset, vector<string> cols) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < cols.size(); i++) {
        new_data.push_back(func_col(dataset, cols[i]));
    }
    return Dataset(new_data);
}

Dataset func_col(Dataset dataset, vector<vector<int>> cols) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < cols.size(); i++) {
        new_data.push_back(func_col(dataset, cols[i]));
    }
    return Dataset(new_data);
}

vector<Dataset_object> func_sum(Dataset dataset, vector<vector<int>> temp) {
    vector<Dataset_object> sum;
    for (int i = 0; i < dataset.data[0].size(); i++) {
        Dataset_object s = Dataset_object(0);
        for (int j = 0; j < temp.size(); j++) {
            s = s + dataset.data[temp[j][0]][i];
        }
        sum.push_back(s);
    }
    return sum;
}

Dataset_object func_sum(Dataset dataset, vector<int> nums) {
    int sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += dataset.data[nums[i]][0].ival;
    }
    return Dataset_object(sum);
}

int func_sum(Dataset dataset, vector<int> nums) {
    int sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
    }
    return sum;
}

float func_sum(Dataset dataset, vector<float> nums) {
    float sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
    }
    return sum;
}

vector<Dataset_object> func_max(Dataset dataset, vector<vector<int>> temp) {
    vector<Dataset_object> max;
    for (int i = 0; i < dataset.data[0].size(); i++) {
        Dataset_object m = dataset.data[temp[0][0]][i];
        for (int j = 1; j < temp.size(); j++) {
            if (dataset.data[temp[j][0]][i].ival > m.ival) {
                m = dataset.data[temp[j][0]][i];
            }
        }
        max.push_back(m);
    }
    return max;
}

Dataset_object func_max(Dataset dataset, vector<int> nums) {
    int m = dataset.data[nums[0]][0].ival;
    for (int i = 1; i < nums.size(); i++) {
        if (dataset.data[nums[i]][0].ival > m) {
            m = dataset.data[nums[i]][0].ival;
        }
    }
    return Dataset_object(m);
}

int func_max(Dataset dataset, vector<int> nums) {
    int m = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] > m) {
            m = nums[i];
        }
    }
    return m;
}

float func_max(Dataset dataset, vector<float> nums) {
    float m = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] > m) {
            m = nums[i];
        }
    }
    return m;
}

vector<Dataset_object> func_min(Dataset dataset, vector<vector<int>> temp) {
    vector<Dataset_object> min;
    for (int i = 0; i < dataset.data[0].size(); i++) {
        Dataset_object m = dataset.data[temp[0][0]][i];
        for (int j = 1; j < temp.size(); j++) {
            if (dataset.data[temp[j][0]][i].ival < m.ival) {
                m = dataset.data[temp[j][0]][i];
            }
        }
        min.push_back(m);
    }
    return min;
}

Dataset_object func_min(Dataset dataset, vector<int> nums) {
    int m = dataset.data[nums[0]][0].ival;
    for (int i = 1; i < nums.size(); i++) {
        if (dataset.data[nums[i]][0].ival < m) {
            m = dataset.data[nums[i]][0].ival;
        }
    }
    return Dataset_object(m);
}

int func_min(Dataset dataset, vector<int> nums) {
    int m = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] < m) {
            m = nums[i];
        }
    }
    return m;
}

float func_min(Dataset dataset, vector<float> nums) {
    float m = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] < m) {
            m = nums[i];
        }
    }
    return m;
}

vector<Dataset_object> func_mean(Dataset dataset, vector<vector<int>> temp) {
    vector<Dataset_object> mean;
    for (int i = 0; i < dataset.data[0].size(); i++) {
        Dataset_object m = Dataset_object(0);
        for (int j = 0; j < temp.size(); j++) {
            m = m + dataset.data[temp[j][0]][i];
        }
        m = m / temp.size();
        mean.push_back(m);
    }
    return mean;
}

Dataset_object func_mean(Dataset dataset, vector<int> nums) {
    int sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += dataset.data[nums[i]][0].ival;
    }
    return Dataset_object(sum / nums.size());
}

int func_mean(Dataset dataset, vector<int> nums) {
    int sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
    }
    return sum / nums.size();
}

float func_mean(Dataset dataset, vector<float> nums) {
    float sum = 0;
    for (int i = 0; i < nums.size(); i++) {
        sum += nums[i];
    }
    return sum / nums.size();
}

Dataset func_join(Dataset dataset1, Dataset dataset2) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < dataset1.data.size(); i++) {
        vector<Dataset_object> row = dataset1.data[i];
        for (int j = 0; j < dataset2.data[i].size(); j++) {
            row.push_back(dataset2.data[i][j]);
        }
        new_data.push_back(row);
    }
    return Dataset(new_data);
}

Dataset func_join(Dataset dataset1, Dataset dataset2, vector<pair<int, int>> temp) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < dataset1.data.size(); i++) {
        vector<Dataset_object> row = dataset1.data[i];
        for (int j = 0; j < temp.size(); j++) {
            row.push_back(dataset2.data[i][temp[j].second]);
        }
        new_data.push_back(row);
    }
    return Dataset(new_data);
}

Dataset func_unique(Dataset dataset) {
    vector<vector<Dataset_object>> new_data;
    for (int i = 0; i < dataset.data.size(); i++) {
        bool unique = true;
        for (int j = 0; j < new_data.size(); j++) {
            if (dataset.data[i] == new_data[j]) {
                unique = false;
                break;
            }
        }
        if (unique) {
            new_data.push_back(dataset.data[i]);
        }
    }
    return Dataset(new_data);
}

vector<Dataset_object> func_unique(vector<Dataset_object> dataset) {
    vector<Dataset_object> new_data;
    for (int i = 0; i < dataset.size(); i++) {
        bool unique = true;
        for (int j = 0; j < new_data.size(); j++) {
            if (dataset[i] == new_data[j]) {
                unique = false;
                break;
            }
        }
        if (unique) {
            new_data.push_back(dataset[i]);
        }
    }
    return new_data;
}

vector<Dataset> func_split(Dataset dataset, vector<vector<int>> temp) {

}

Dataset func_sort(Dataset dataset, vector<vector<int>> temptemp) {
    vector<int> temp = temptemp[0];
    for(int i=0; i<temp.size(); i++) {
        vector<Dataset_object> col = func_col(dataset, temp[i]);
        sort(col.begin(), col.end());
        for(int j=0; j<dataset.data.size(); j++) {
            dataset.data[j][temp[i]] = col[j];
        }
    }
    return dataset;
}

Dataset func_sort(Dataset dataset){
    for(int i=0; i<dataset.data[0].size(); i++) {
        vector<Dataset_object> col = func_col(dataset, i);
        sort(col.begin(), col.end());
        for(int j=0; j<dataset.data.size(); j++) {
            dataset.data[j][i] = col[j];
        }
    }
    return dataset;
}

vector<int> func_sort(vector<int> nums) {
    sort(nums.begin(), nums.end());
    return nums;
}

vector<float> func_sort(vector<float> nums) {
    sort(nums.begin(), nums.end());
    return nums;
}

vector<string> func_sort(vector<string> nums) {
    sort(nums.begin(), nums.end());
    return nums;
}

vector<bool> func_sort(vector<bool> nums) {
    sort(nums.begin(), nums.end());
    return nums;
}

vector<char> func_sort(vector<char> nums) {
    sort(nums.begin(), nums.end());
    return nums;
}

Dataset func_shuffle(Dataset dataset) {
    for(int i=0; i<dataset.data[0].size(); i++) {
        vector<Dataset_object> col = func_col(dataset, i);
        random_shuffle(col.begin(), col.end());
        for(int j=0; j<dataset.data.size(); j++) {
            dataset.data[j][i] = col[j];
        }
    }
    return dataset;
}

vector<int> func_shuffle(vector<int> nums) {
    random_shuffle(nums.begin(), nums.end());
    return nums;
}

vector<float> func_shuffle(vector<float> nums) {
    random_shuffle(nums.begin(), nums.end());
    return nums;
}

vector<string> func_shuffle(vector<string> nums) {
    random_shuffle(nums.begin(), nums.end());
    return nums;
}

vector<bool> func_shuffle(vector<bool> nums) {
    random_shuffle(nums.begin(), nums.end());
    return nums;
}

vector<char> func_shuffle(vector<char> nums) {
    random_shuffle(nums.begin(), nums.end());
    return nums;
}

Dataset func_add(Dataset dataset, vector<vector<Dataset_object>> temp) {
    for(int i=0; i<temp.size(); i++) {
        dataset.data.push_back(temp[i]);
    }
    return dataset;
}

vector<Dataset_object> func_add(vector<Dataset_object> data, vector<Dataset_object> temp) {
    for(int i=0; i<temp.size(); i++) {
        data.push_back(temp[i]);
    }
    return data;
}

vector<int> func_shape(Dataset dataset) {
    return {dataset.data.size(), dataset.data[0].size()};
}

int func_shape(vector<int> nums) {
    return nums.size();
}

void func_drop(Dataset dataset) {
    dataset.data.clear();
}

void func_drop(Dataset dataset, vector<vector<int>> temp) {
    for(int i=0; i<temp.size(); i++) {
        dataset.data.erase(dataset.data.begin() + temp[i][0]);
    }
}

void func_show_bar(vector<Dataset_object> data, vector<vector<int>> temp) {
    vector<string> labels;
    vector<int> values;
    for(int i=0; i<temp.size(); i++) {
        labels.push_back(data[temp[i][0]].sval);
        values.push_back(data[temp[i][1]].ival);
    }
    plt::bar(labels, values);
    plt::show();
}

void func_show_bar(vector<Dataset_object> data, vector<vector<float>> temp) {
    vector<string> labels;
    vector<float> values;
    for(int i=0; i<temp.size(); i++) {
        labels.push_back(data[temp[i][0]].sval);
        values.push_back(data[temp[i][1]].fval);
    }
    plt::bar(labels, values);
    plt::show();
}

void func_show_bar(vector<Dataset_object> data) {
    vector<string> labels;
    vector<int> values;
    for(int i=0; i<data.size(); i++) {
        labels.push_back(data[i].sval);
        values.push_back(data[i].ival);
    }
    plt::bar(labels, values);
    plt::show();
}
void func_show_box(vector<Dataset_object> data, vector<vector<int>> temp) {
    vector<string> labels;
    vector<vector<int>> values;
    
    for (int i = 0; i < temp.size(); i++) {
        labels.push_back(data[temp[i][0]].sval);
        vector<int> columnValues;
        for (int j = 1; j < temp[i].size(); j++) {
            columnValues.push_back(data[temp[i][j]].ival);
        }
        values.push_back(columnValues);
    }
    
    plt::boxplot(values, labels);
    plt::show();
}

void func_show_box(vector<Dataset_object> data, vector<vector<float>> temp) {
    vector<string> labels;
    vector<vector<float>> values;

    for (int i = 0; i < temp.size(); i++) {
        labels.push_back(data[temp[i][0]].sval);
        vector<float> columnValues;
        for (int j = 1; j < temp[i].size(); j++) {
            columnValues.push_back(data[temp[i][j]].fval);
        }
        values.push_back(columnValues);
    }

    plt::boxplot(values, labels);
    plt::show();
}show_bar

void func_show_box(vector<Dataset_object> data) {
    vector<string> labels;
    vector<vector<float>> values;

    for (int i = 0; i < data.size(); i++) {
        labels.push_back(data[i].sval);
        vector<float> columnValues;

        if (data[i].type == Dataset_object::DataType::Integer) {
            columnValues.push_back(static_cast<float>(data[i].ival));
        } else if (data[i].type == Dataset_object::DataType::Float) {
            columnValues.push_back(data[i].fval);
        }
        values.push_back(columnValues);
    }

    plt::boxplot(values, labels);
    plt::show();
}
void func_show_line(vector<Dataset_object> data, vector<pair<int, int>> temp) {
    for (const auto& pair : temp) {
        vector<float> xValues;
        vector<float> yValues;

        for (int i = 0; i < data.size(); i++) {
            if (data[i].type == Dataset_object::DataType::Float || data[i].type == Dataset_object::DataType::Integer) {
                xValues.push_back(data[i][pair.first].fval);
                yValues.push_back(data[i][pair.second].fval);
            }
        }
        plt::plot(xValues, yValues);
    }
    plt::show();
}

void func_show_line(vector<Dataset_object> data) {
    vector<float> xValues;
    vector<float> yValues;

    for (int i = 0; i < data.size(); i++) {
        if (data[i].type == Dataset_object::DataType::Float || data[i].type == Dataset_object::DataType::Integer) {
            xValues.push_back(static_cast<float>(data[i].ival));
            yValues.push_back(data[i].fval);
        }
    }
    plt::plot(xValues, yValues);
    plt::show();
}
void func_show_scatter(vector<Dataset_object> data, vector<pair<int, int>> temp) {
    for (const auto& pair : temp) {
        vector<float> xValues;
        vector<float> yValues;

        for (int i = 0; i < data.size(); i++) {
            if (data[i].type == Dataset_object::DataType::Float || data[i].type == Dataset_object::DataType::Integer) {
                xValues.push_back(data[i][pair.first].fval);
                yValues.push_back(data[i][pair.second].fval);
            }
        }
        plt::scatter(xValues, yValues);
    }
    plt::show();
}

void func_show_scatter(vector<Dataset_object> data) {
    vector<float> xValues;
    vector<float> yValues;

    for (int i = 0; i < data.size(); i++) {
        if (data[i].type == Dataset_object::DataType::Float || data[i].type == Dataset_object::DataType::Integer) {
            xValues.push_back(static_cast<float>(data[i].ival));
            yValues.push_back(data[i].fval);
        }
    }
    plt::scatter(xValues, yValues);
    plt::show();
}



Dataset func_read(vector<string> temp) {
    // read the dataset from the csv
    filename = temp[0];
    ifstream file(filename);
    string line;
    vector<vector<Dataset_object>> data;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<Dataset_object> row;
        while (getline(ss, cell, ',')) {
            try {
                row.push_back(Dataset_object(stoi(cell)));
            } catch (invalid_argument e) {
                try {
                    row.push_back(Dataset_object(stof(cell)));
                } catch (invalid_argument e) {
                    if (cell == "true") {
                        row.push_back(Dataset_object(true));
                    } else if (cell == "false") {
                        row.push_back(Dataset_object(false));
                    } else {
                        row.push_back(Dataset_object(cell));
                    }
                }
            }
        }
        data.push_back(row);
    }
    return Dataset(data);
}

void func_write(Dataset dataset, vector<string> temp) {
    // write the dataset to the csv
    filename = temp[0];
    ofstream file(filename);
    for (int i = 0; i < dataset.data.size(); i++) {
        for (int j = 0; j < dataset.data[i].size(); j++) {
            if (dataset.data[i][j].type == 1) {
                file << dataset.data[i][j].ival;
            } else if (dataset.data[i][j].type == 2) {
                file << dataset.data[i][j].fval;
            } else if (dataset.data[i][j].type == 3) {
                file << dataset.data[i][j].bval;
            } else if (dataset.data[i][j].type == 4) {
                file << dataset.data[i][j].sval;
            }
            if (j != dataset.data[i].size() - 1) {
                file << ",";
            }
        }
        file << endl;
    }
}

void func_show(Dataset dataset) {
    cout << "Dataset:" << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < dataset.data.size(); i++) {
        for (int j = 0; j < dataset.data[i].size(); j++) {
            if (dataset.data[i][j].type == 1) {
                cout << dataset.data[i][j].ival;
            } else if (dataset.data[i][j].type == 2) {
                cout << dataset.data[i][j].fval;
            } else if (dataset.data[i][j].type == 3) {
                cout << dataset.data[i][j].bval;
            } else if (dataset.data[i][j].type == 4) {
                cout << dataset.data[i][j].sval;
            }
            if (j != dataset.data[i].size() - 1) {
                cout << ",";
            }
        }
        cout << endl;
    }
    cout << "----------------------------------------------" << endl;
    vector<int> shape = func_shape(dataset);
    cout << "Shape: (" << shape[0] << ", " << shape[1] << ")" << endl;
}

void func_show(vector<Dataset_object> data) {
    cout << "Array:" << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < data.size(); i++) {
        if (data[i].type == 1) {
            cout << data[i].ival;
        } else if (data[i].type == 2) {
            cout << data[i].fval;
        } else if (data[i].type == 3) {
            cout << data[i].bval;
        } else if (data[i].type == 4) {
            cout << data[i].sval;
        }
        if (i != data.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;
    cout << "----------------------------------------------" << endl;
}

void func_show(vector<int> data) {
    cout << "Array of Integers:" << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
        if (i != data.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;
    cout << "----------------------------------------------" << endl;
}

void func_show(vector<float> data) {
    cout << "Array of Floats:" << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
        if (i != data.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;
    cout << "----------------------------------------------" << endl;
}

void func_show(vector<string> data) {
    cout << "Array of Strings:" << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
        if (i != data.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;
    cout << "----------------------------------------------" << endl;
}

void func_show(vector<bool> data) {
    cout << "Array of Booleans:" << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
        if (i != data.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;
    cout << "----------------------------------------------" << endl;
}

void func_show(vector<char> data) {
    cout << "Array of Characters:" << endl;
    cout << "----------------------------------------------" << endl;
    for (int i = 0; i < data.size(); i++) {
        cout << data[i];
        if (i != data.size() - 1) {
            cout << ",";
        }
    }
    cout << endl;
    cout << "----------------------------------------------" << endl;
}

void func_show(int data) {
    cout << "Integer: " << data << endl;
}

void func_show(float data) {
    cout << "Float: " << data << endl;
}

void func_show(string data) {
    cout << "String: " << data << endl;
}

void func_show(bool data) {
    cout << "Boolean: " << data << endl;
}

void func_show(char data) {
    cout << "Character: " << data << endl;
}