#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ROWS 100  // 假设最多100行数据


//// 解析 "YYYY-MM-DD HH:MM" 格式的时间字符串为 time_t
//time_t parse_time(const char *time_str) {
//    struct tm tm_time = {0};
//    if (sscanf(time_str, "%d-%d-%d %d:%d",
//               &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday,
//               &tm_time.tm_hour, &tm_time.tm_min) != 5) {
//        return -1;  // 解析失败
//    }
//    tm_time.tm_year -= 1900;
//    tm_time.tm_mon -= 1;
//    return mktime(&tm_time);  // 转换为时间戳
//}

int convert_time_to_int(const char *time_str) {
    struct tm tm_time = {0};

    // Parse the input string in the format "YYYY-MM-DD HH:MM"
    if (sscanf(time_str, "%d-%d-%d %d:%d",
               &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday,
               &tm_time.tm_hour, &tm_time.tm_min) != 5) {
        printf("Time parsing error. input=%s\n", time_str);
        return -1;  // Return an error code if parsing fails
    }

    tm_time.tm_year -= 1900;  // Year since 1900
    tm_time.tm_mon -= 1;      // Month is 0-11

    // Convert to time_t (seconds since Unix epoch)
    time_t raw_time = mktime(&tm_time);
    if (raw_time == -1) {
        printf("Error converting time to seconds.\n");
        return -1;
    }

    return (int)raw_time;  // Return the time as an integer (seconds)
}


int main() {
    // 定义变量
    FILE *file = fopen("electricity_irrigation_basic_input.txt", "r");
    if (file == NULL) {
        printf("无法打开electricity_irrigation_basic_input.txt文件！\n");
        return 1;
    }

    char line[256];
    double rated_head_m = 0, rated_flow = 0, input_power_kw = 0;
    double pump_efficiency = 0, turbine_efficiency = 0, generating_efficiency = 0;
    double price_low_electricity = 0, price_up_electricity = 0;
    double yield = 0, water_use_per_mu = 0, rice_price = 0;
    
    
    while (fgets(line, sizeof(line), file)) {
        // 去除换行符（非常重要）
        line[strcspn(line, "\r\n")] = 0;

        if (sscanf(line, "rated_head_m %lf", &rated_head_m) == 1) continue;
        if (sscanf(line, "rated_flow %lf", &rated_flow) == 1) continue;
        if (sscanf(line, "input_power_kw %lf", &input_power_kw) == 1) continue;
        if (sscanf(line, "pump_efficiency %lf", &pump_efficiency) == 1) continue;
        if (sscanf(line, "turbine_efficiency %lf", &turbine_efficiency) == 1) continue;
        if (sscanf(line, "generating_efficiency %lf", &generating_efficiency) == 1) continue;
        if (sscanf(line, "price_low_electricity %lf", &price_low_electricity) == 1) continue;
        if (sscanf(line, "price_up_electricity %lf", &price_up_electricity) == 1) continue;
        if (sscanf(line, "yield %lf", &yield) == 1) continue;
        if (sscanf(line, "water_use_per_mu %lf", &water_use_per_mu) == 1) continue;
        if (sscanf(line, "rice_price %lf", &rice_price) == 1) continue;

    }

    fclose(file);

    // 输出读取的数据
    printf("rated_head_m: %.2lf\n", rated_head_m);
    printf("rated_flow: %.2lf\n", rated_flow);
    printf("input_power_kw: %.2lf\n", input_power_kw);
    printf("pump_efficiency: %.2lf\n", pump_efficiency);
    printf("turbine_efficiency: %.2lf\n", turbine_efficiency);
    printf("generating_efficiency: %.2lf\n", generating_efficiency);
    printf("price_low_electricity: %.3lf\n", price_low_electricity);
    printf("price_up_electricity: %.3lf\n", price_up_electricity);
    printf("yield: %.2lf\n",yield);
    printf("water_use_per_mu: %.2lf\n", water_use_per_mu);
    printf("rice_price: %.2lf\n",rice_price);

    

    //--------------------读取pump和time数据-------------------------------//
    //打开文件
    FILE *csv_file = fopen("electricity_irrigation_flow_data.txt", "r");
    if (!csv_file) {
        printf("electricity_irrigation_flow_data.txt文件！\n");
        return 1;
    }

    int total_pump[MAX_ROWS] = {0};  // 存储电站机组（台）数变化（台数）
    double electricity_time_diff[MAX_ROWS -1] = {0};  // 存储电站时间差（小时）
    int pump_units[MAX_ROWS] = {0};  // 存储电站机组数（第1列）
    double electricity_flow[MAX_ROWS] = {0.0}; //存储发电流量数（第2列）
    time_t electricity_time[MAX_ROWS] = {0}; // 存储电站时间戳（第3列）
    double irrigation_time_diff[MAX_ROWS -1] = {0};  // 存储灌溉时间差（小时）
    double irrigation_flow[MAX_ROWS] = {0.0};  // 存储灌溉流量数（第4列）
    time_t irrigation_time[MAX_ROWS] = {0}; // 存储灌溉时间戳（第5列）


    int row = 0;  // 当前行号

    // 读取表头（跳过第一行）
    fgets(line, sizeof(line), csv_file);

    // 读取数据
    while (fgets(line, sizeof(line), csv_file) && row < MAX_ROWS) {
        char irrigation_time_str[300], electricity_time_str[300];

        // 解析time_flow数据
        //if (sscanf(line, "%d %lf %s %lf %s",
        if (sscanf(line, "%d %lf %63[^\t] %lf %63[^\n]",
                   &pump_units[row],
                   &electricity_flow[row],
                   electricity_time_str,
                   &irrigation_flow[row],
                   irrigation_time_str) == 5) {
            //electricity_time[row] = parse_time(electricity_time_str);
            //irrigation_time[row] = parse_time(irrigation_time_str);
            electricity_time[row] = convert_time_to_int(electricity_time_str);
            irrigation_time[row] = convert_time_to_int(irrigation_time_str);
            row++;
        } else {
            printf("第 %d 行数据格式不正确，跳过\n", row + 1);
        }
    }

    fclose(csv_file);
    
    

    // 计算时间差
    // 计算相邻两行的时间差
     for (int i = 0; i < row - 1; i++) {
         electricity_time_diff[i] = (electricity_time[i] - electricity_time[i+1]) / 3600.0;
         irrigation_time_diff[i] = (irrigation_time[i] - irrigation_time[i+1]) / 3600.0;
     }

    // 输出时间差数组
     printf("\n电站流量时间差 (小时):\n");
     for (int i = 0; i < row - 1; i++) {
         printf("行 %d 和 %d: %.2f 小时\n", i + 1, i + 2, electricity_time_diff[i]);
     }

     printf("\n灌溉流量时间差 (小时):\n");
     for (int i = 0; i < row - 1; i++) {
         printf("行 %d 和 %d: %.2f 小时\n", i + 1, i + 2, irrigation_time_diff[i]);
    }
    
    //----------------------------发电效益计算--------------------------------------//
    double total_energy_consumption_sum = 0;  // 总抽水耗能
    double total_power_generation_capacity_sum = 0; // 总发电量

    // 计算单次发电功率P =η⋅ρ⋅g⋅Q⋅H
    double generated_output = 1 * 9.81 * rated_flow * rated_head_m * turbine_efficiency;

    // 计算单次发电量
    double power_generation_capacity = generated_output * generating_efficiency;

    // 遍历数组计算累加值
    for (int i = 0; i < row-1; i++) {
        double total_energy_consumption = input_power_kw * electricity_time_diff[i] * pump_units[i+1];
        double total_power_generation_capacity = power_generation_capacity * electricity_time_diff[i] * pump_units[i+1];

        total_energy_consumption_sum += total_energy_consumption;
        total_power_generation_capacity_sum += total_power_generation_capacity;
    }

    // 计算最终发电效益
    double benefit_power = total_power_generation_capacity_sum * price_up_electricity;

    // 输出结果
    //printf("总抽水耗能: %.2f kWh\n", total_energy_consumption_sum);
    printf("electricity_produce: %.2f kWh\n", total_power_generation_capacity_sum);
    printf("electricity_benefit: %.2f 元\n", benefit_power);

  
  
    //-----------------------------灌溉效益计算----------------------------------//
    // 计算单位产值耗水量
    double water_benefit_per_m3=water_use_per_mu/yield;

    // 遍历数组计算耗水量累加值
    double total_water_consumption_sum= 0; // 总耗水量

    for (int i = 0; i < row-1; i++) {
        double total_water_consumption = 3600 * irrigation_time_diff[i] * irrigation_flow[i+1];

        total_water_consumption_sum += total_water_consumption;
     }
    // 计算最终灌溉效益
    double benefit_irrigation =water_benefit_per_m3 * total_water_consumption_sum * rice_price;

    // 输出结果
    //printf("单位产值耗水量: %.2f m3/kg\n", water_benefit_per_m3);
    printf("irrigation_water: %.2f m3\n", total_water_consumption_sum);
    printf("irrigation_benefit: %.2f 元\n", benefit_irrigation);


   
    //--------------------------- 将结果保存到文件----------------------//
    FILE *output_file = fopen("electricity_irrigation_results.txt", "w");
    if (!output_file) {
        printf("无法创建 electricity_irrigation_results.txt 文件！\n");
        return 1;
    }

    //fprintf(output_file, "总抽水耗能: %.2f kWh\n", total_energy_consumption_sum);
    fprintf(output_file, "electricity_produce: %.2f kWh\n", total_power_generation_capacity_sum);
    fprintf(output_file, "electricity_benefit: %.2f 元\n", benefit_power);
    
    //fprintf(output_file, "单位产值耗水量: %.2f m3/kg\n", water_benefit_per_m3);
    fprintf(output_file, "irrigation_water: %.2f m3\n", total_water_consumption_sum);
    fprintf(output_file, "irrigation_benefit: %.2f 元\n", benefit_irrigation);

    fclose(output_file);
    printf("计算结果已保存至 electricity_irrigation_results.txt\n");
    
    return 0;
}

