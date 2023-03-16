/*
* Created by boil on 2023/2/4.
*/
/*
 * @Description: 银行贷款等额本息和等额本金的还款方式
 * @Author: szq
 * @Github: https://github.com/MrQqqq
 * @Date: 2020-07-01 09:32:19
 * @LastEditors: szq
 * @LastEditTime: 2020-07-01 12:01:45
 * @FilePath: \cpp\src\bank_loan.cpp
 */
#include<iostream>
#include<math.h>
#include<iomanip>

using namespace std;

/**
 * @destription: 等额本息每个月还款额和还款总额（本金加总利息）
 * @param {type} ：principal：本金   months：贷款总月数  rate:年利率
 * @return: 没有返回值
 */
double average_capital_plus_interest(double principal, int months, double rate) {
  double total_per_month;//等额本息每个月还款总额
  double total_principal_interest;//本金和利息总额
  double rate_month = rate / 12;//月利率
  total_per_month = principal * rate_month * pow(rate_month + 1, months)
      / (pow(rate_month + 1, months) - 1);
  total_principal_interest = total_per_month * months;
  cout << "等额本息还款详情为：" << endl;
  cout << "每个月还款额为:" << setiosflags(ios::fixed) << setprecision(2) << total_per_month << endl;
  cout << "还款总额为：" << setiosflags(ios::fixed) << setprecision(2) << total_principal_interest << endl;
  return total_per_month;
}

/**
 * @destription: 等额本金每个月还款额和还款总额（本金加总利息）
 * @param {type} ：principal：本金   months：贷款总月数  rate:年利率
 * @return: 没有返回值
 */

void average_interest(double principal, int months, double rate, double vs) {
  double principal_per_month = principal / months;//等额本金每个月还款本金
  double total_principal_interest = 0;//本金和利息总额
  double rate_month = rate / 12;//月利率
  cout << "等额本金还款详情为：" << endl;
  bool show = true;
  for (int i = 1; i <= months; i++) {
    double total_per_month = principal * rate_month + principal_per_month;//每期还款总额
    principal = principal - principal_per_month;//剩余本金
    cout << "第" << i << "个月还款金额为：" << setiosflags(ios::fixed) << setprecision(2) << total_per_month << endl;
    total_principal_interest += total_per_month;
    if (total_per_month <= vs && show) {
      cout << "第" << i << "个月还款金额与等额本息持平为：" << setiosflags(ios::fixed) << setprecision(2)
           << total_per_month << endl;
      show = false;
    }
  }
  cout << "还款总额为：" << setiosflags(ios::fixed) << setprecision(2) << total_principal_interest << endl;
}


int main() {
  double principal = 2000000.0;//本金
  double rate = 0.035;//年利率
  double months = 240;//贷款月数
  cout << "请输入贷款额度:" << setiosflags(ios::fixed) << setprecision(3) << principal << endl;
//  cin >> principal;
  cout << endl;

  cout << "请输入贷款期限（单位：月）:" << months << endl;
//  cin >> months;
  cout << endl;

  cout << "请输入贷款年利率:" << rate << endl;
//  cin >> rate;
  cout << endl;
  double result = average_capital_plus_interest(principal, months, rate);
  average_interest(principal, months, rate, result);

//  int choice = 1;//还款方式,1为等额本息，2为等额本金
//  cout << "1.等额本息" << endl;
//  cout << "2.等额本金" << endl;
//  cout << "请选择还款方式：";
//  cin >> choice;
//  cout << endl;
//
//  switch (choice) {
//    case 1:
//      average_capital_plus_interest(principal, months, rate);
//      break;
//    case 2:
//      average_interest(principal, months, rate);
//      break;
//    default:
//
//      break;
//  }
}
