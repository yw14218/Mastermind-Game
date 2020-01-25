#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

void set_random_seed();
int randn(int n);
void compa_attem_and_possi_code(int num, const std::vector<int> attempt, const std::vector<int> possible_code, int& black_hits, int& white_hits);

struct mm_code_maker{

    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
    }

    void generate_sequence(){
        for(int i = 0; i < length; i++){
            sequence.push_back(randn(num));
        }
    }

    void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits){
        black_hits = 0;
        white_hits = 0;
        std::vector<int> sequence_copy;
        std::vector<int> attempt_copy;
        int size = sequence.size();

        //copy:
        for(int i = 0; i < size; i++){
           int tmp = sequence[i];
           sequence_copy.push_back(tmp);
        }

        for(int i = 0; i < size; i++){
           int tmp = attempt[i];
           attempt_copy.push_back(tmp);
        }

        //computing blackhits:
        for(int j = 0; j < size; j++){
           if(attempt_copy[j] == sequence_copy[j]){
              black_hits++;
              sequence_copy[j] = num;
              attempt_copy[j] = num + 1;
           }
        }
        //computing whitehits:
        if(black_hits != size){
            for(int p = 0; p < size; p++){
               bool found = false;
               for(int q = 0; q < size; q++){
                   if(attempt_copy[p] == sequence_copy[q] && found == false){
                       white_hits++;
                       sequence_copy[q] = num;
                       attempt_copy[p] = num + 1;
                       found = true;
                   }
               }
            }
        }

    }

    std::vector<int> sequence;

    int length;
    int num;
};

struct mm_solver{

    std:: vector<std:: vector <int>> space;
    std:: vector<std:: vector <int>> not_correct;
    std:: vector<int> code_high_case;
    std:: vector<int> final_code;


    int size = pow(num, length);

    void cre_possi_space(int lenghth, int num, std::vector <std::vector<int>> &space){
        std:: vector<int> code;
        int size = pow(num, lenghth);
        for(int i = 0; i < size; i++){
            int tmp = i;
            for(int i = 0; i < lenghth; i++){
                code.push_back(tmp % num);
                tmp = tmp / num;
            }
            space.push_back(code);
            code.clear();
        }
    }

    void init(int i_length, int i_num){
        length = i_length;
        num = i_num;
        case_main_part = length;
        case_remain = length % 3;
    }


    void create_attempt(std::vector<int>& attempt){
       //small instance use Donald Knuth
       if(pow(num, length) <= 10000){
           if(space.empty()){
             cre_possi_space(length, num, space);
           }

           int index = space.size();
           attempt = space[randn(index)];

           bool ready = false;
           while(!ready){

               int index = space.size();
               attempt = space[randn(index)];
               bool found = false;
               for(unsigned i = 0; i < not_correct.size() && !found; i++){
                   if(attempt == not_correct[i]){
                       found = true;
                   }
               }

               if(found){
                   attempt.clear();
               }

               else{
                   ready = true;
               }
           }
       }

       //certain instances within spanned range use Donald to solve three codes at a time
       else if((length == 6 && num >= 9 && num <= 15) ||(length == 9 && num >= 9 && num <= 15) || (length == 12 && num >= 9 && num <= 15)  || (length == 15 && num == 15)){

            if(index == -1){

               for(int i = 0; i < length; i++){
                   attempt.push_back(0);
               }
            }

            else{
               for(int i = 0; i < length; i++){
                   attempt.push_back(0);
               }
               attempt[index] = 1;
            }

            if(index == length){
               attempt.clear();
               change_to_incorrect(code_high_case);
               for(unsigned i = 0; i < code_high_case.size(); i++){
                   attempt.push_back(code_high_case[i]);
               }
               index++;
            }

            if(index > length && case_main_part >= 3){
               generate_attempt_high_case(case_index, attempt);
            }

            else if(index > length && case_main_part == 0){
               attempt = final_code;
            }
       }



       //high instances use variable controlling
       else{
           if(index == -1){
              for(int i = 0; i < length; i++){
                  attempt.push_back(0);
              }
           }

           else {
                 bool ready = false;
                 while(!ready){

                     for(int i = 0; i < length; i++){
                          attempt.push_back(0);
                     }
                     attempt[index] = randn(num);

                     bool found = false;
                     for(unsigned i = 0; i < not_correct.size() && !found; i++){
                           if(attempt == not_correct[i]){
                           found = true;
                           }
                     }

                     if(index == length){

                       attempt.clear();
                       ready = true;
                       found = false;
                       for(unsigned i = 0; i < code_high_case.size(); i++){
                           attempt.push_back(code_high_case[i]);
                       }
                     }

                     if(found){
                           attempt.clear();
                     }
                     else{
                           ready = true;
                     }

                  }

            }

       }



    }

    //this function stores all the incorrect element in a vector so that they wont influence the later test
    void change_to_incorrect(std::vector<int>& attempt){
      for(unsigned i = 0; i < code_high_case.size(); i++){
         final_code.push_back(num);
      }

      for(unsigned i = 0; i < code_high_case.size(); i++){
         if(code_high_case[i] == 0){
             code_high_case[i] = 1;
         }

         else if(code_high_case[i] == 1){
             code_high_case[i] = 0;
         }

         else{
             code_high_case[i] = 0;
         }
      }

    }


    void generate_attempt_high_case(int case_index, std::vector<int>& attempt){

          if(space.empty() && num > 8){
            cre_possi_space(3, num, space);
          }

          int index = space.size();
          attempt = space[randn(index)];
          bool ready = false;
          while(!ready){

              int index = space.size();
              attempt = space[randn(index)];

              bool found = false;
              for(unsigned i = 0; i < not_correct.size() && !found; i++){
                  if(attempt == not_correct[i]){
                      found = true;
                  }
              }

              if(found){
                  attempt.clear();
              }

              else{
                  ready = true;
                  if(case_index == 0){
                      for(unsigned i = 0; i + 3 < code_high_case.size(); i++)
                           attempt.push_back(code_high_case[i+3]);
                      }

                  if(case_index == 1){
                      std::vector <int> tmp;
                      for(unsigned i = 0; i < code_high_case.size();i++){
                          tmp.push_back(code_high_case[i]);
                      }
                      tmp[3] = attempt[0];
                      tmp[4] = attempt[1];
                      tmp[5] = attempt[2];
                      attempt = tmp;
                  }

                  if(case_index == 2){
                      std::vector <int> tmp;
                      for(unsigned i = 0; i < code_high_case.size();i++){
                          tmp.push_back(code_high_case[i]);
                      }
                      tmp[6] = attempt[0];
                      tmp[7] = attempt[1];
                      tmp[8] = attempt[2];
                      attempt = tmp;
                  }

                  if(case_index == 3){
                      std::vector <int> tmp;
                      for(unsigned i = 0; i < code_high_case.size();i++){
                          tmp.push_back(code_high_case[i]);
                      }
                      tmp[9] = attempt[0];
                      tmp[10] = attempt[1];
                      tmp[11] = attempt[2];
                      attempt = tmp;
                  }
                  if(case_index == 4){
                      std::vector <int> tmp;
                      for(unsigned i = 0; i < code_high_case.size();i++){
                          tmp.push_back(code_high_case[i]);
                      }
                      tmp[12] = attempt[0];
                      tmp[13] = attempt[1];
                      tmp[14] = attempt[2];
                      attempt = tmp;
                  }
                  if(case_index == 5){

                     attempt = final_code;
                  }

              }
          }


    }


    void learn(std::vector<int>& attempt, int black_hits, int white_hits){

      //Donald Knuth
      if(pow(num, length) <= 10000){

         for(unsigned i = 0; i < space.size(); i++){

           int blackhits_compare, whitehits_compare = 0;
           std:: vector <int> tmp = space[i];
           compa_attem_and_possi_code(num, attempt, tmp, blackhits_compare, whitehits_compare);

           if(blackhits_compare != black_hits || whitehits_compare != white_hits){
              not_correct.push_back(tmp);
           }
         }
      }

     //certain instances within spanned range use Donald to solve three codes at a time
      else if((length == 6 && num >= 9 && num <= 15) ||(length == 9 && num >= 9 && num <= 15) || (length == 12 && num >= 9 && num <= 15)  || (length == 15 && num == 15)){
            if(index == -1){
               initial_black = black_hits;
               index ++;
            }

            else if(index < length){
               if(black_hits < initial_black){
                   code_high_case.push_back(0);
                   index++;
               }

               else if(black_hits > initial_black){
                   code_high_case.push_back(attempt[index]);
                   index++;
               }

               else{
                  code_high_case.push_back(num);
                  index++;
               }
            }

            else if(index > length && case_index < 5){
               for(unsigned i = 0; i < space.size(); i++){
                    int blackhits_compare, whitehits_compare = 0;
                    std:: vector <int> tmp = space[i];
                    if(case_index == 0){
                        for(unsigned i = 0; i + 3 < code_high_case.size(); i++)
                            tmp.push_back(code_high_case[i+3]);
                    }
                    if(case_index == 1){
                        std::vector <int> tmpo;
                        for(unsigned i = 0; i < code_high_case.size();i++){
                            tmpo.push_back(code_high_case[i]);
                        }
                        tmpo[3] = tmp[0];
                        tmpo[4] = tmp[1];
                        tmpo[5] = tmp[2];
                        tmp = tmpo;
                    }
                    if(case_index == 2){
                        std::vector <int> tmpo;
                        for(unsigned i = 0; i < code_high_case.size();i++){
                            tmpo.push_back(code_high_case[i]);
                        }
                        tmpo[6] = tmp[0];
                        tmpo[7] = tmp[1];
                        tmpo[8] = tmp[2];
                        tmp = tmpo;
                    }
                    if(case_index == 3){
                        std::vector <int> tmpo;
                        for(unsigned i = 0; i < code_high_case.size();i++){
                            tmpo.push_back(code_high_case[i]);
                        }
                        tmpo[9] = tmp[0];
                        tmpo[10] = tmp[1];
                        tmpo[11] = tmp[2];
                        tmp = tmpo;
                    }
                    if(case_index == 4){
                        std::vector <int> tmpo;
                        for(unsigned i = 0; i < code_high_case.size();i++){
                            tmpo.push_back(code_high_case[i]);
                        }
                        tmpo[12] = tmp[0];
                        tmpo[13] = tmp[1];
                        tmpo[14] = tmp[2];
                        tmp = tmpo;
                    }
                    compa_attem_and_possi_code(num, attempt, tmp, blackhits_compare, whitehits_compare);

                    if(blackhits_compare != (black_hits + length -3)){
                        not_correct.push_back(space[i]);
                    }
                  }
                  if(black_hits == 3){
                      if(case_index == 0){
                         final_code[0] = attempt[0];
                         final_code[1] = attempt[1];
                         final_code[2] = attempt[2];
                         case_main_part -= 3;
                      }
                      if(case_index == 1){
                         final_code[3] = attempt[3];
                         final_code[4] = attempt[4];
                         final_code[5] = attempt[5];
                         case_main_part -= 3;
                      }
                      if(case_index == 2){
                         final_code[6] = attempt[6];
                         final_code[7] = attempt[7];
                         final_code[8] = attempt[8];
                         case_main_part -= 3;
                      }
                      if(case_index == 3){
                         final_code[9] = attempt[9];
                         final_code[10] = attempt[10];
                         final_code[11] = attempt[11];
                         case_main_part -= 3;
                      }
                      if(case_index == 4){
                         final_code[12] = attempt[12];
                         final_code[13] = attempt[13];
                         final_code[14] = attempt[14];
                         case_main_part -= 3;
                      }
                      case_index++;
                      not_correct.clear();
                  }
            }

      }


      //algorithm of controlling variable
      else {

         if(index == -1){
            initial_black = black_hits;
            index ++;
         }

         else{

            if(black_hits < initial_black){
                code_high_case.push_back(0);
                index++;


            }
            else if(black_hits > initial_black){
                code_high_case.push_back(attempt[index]);
                index++;

            }
            else if(black_hits == initial_black){

                not_correct.push_back(attempt);

            }

         }

      }




    }

    int length;
    int num;
    int index = -1;
    int initial_black;
    int case_index = 0;
    int case_main_part;
    int case_remain;


};

/// this is an example of main

int main(){
    // / our program uses random features so we need to call the function setting a random seed

    set_random_seed();

    int length, num;
    std::cout << "enter length of sequence and number of possible values:" << std::endl;
    std::cin >> length >> num;

    mm_solver solver;
    /// we declare an object of type mm_solver
    solver.init(length, num);
    /// we initialise the values for length and num
    mm_code_maker maker;
    /// we declare an object of type mm_code_maker
    maker.init(length, num);
    /// we initialise the values for length and num
    maker.generate_sequence();
    /// we generate a random sequence
    int black_hits = 0, white_hits = 0;
    int attempts_limit = 5000;
    /// just some number to limit the number of attempts
    int attempts = 0;

    while((black_hits < length) && (attempts < attempts_limit)){
        std::vector<int> attempt;
        solver.create_attempt(attempt);
        /// the solver creates an attempt
        maker.give_feedback(attempt, black_hits, white_hits);
        /// we ask for feedback for the attempt
        /// we print the attempt
        std::cout << "attempt: " << std::endl;
        for(unsigned i = 0; i < attempt.size(); i++){
            std::cout << attempt[i] << " ";
        }
        std::cout << std::endl;
        /// we print the feedback
        std::cout << "black pegs: " << black_hits << " " << " white pegs: " << white_hits << std::endl;
        /// we give the feedback to the solver so that it can learn
        solver.learn(attempt, black_hits, white_hits);
        attempts++;
    }

    if(black_hits == length){
        std::cout << "the solver has found the sequence in " << attempts << " attempts" << std::endl;
    }
    else{
        std::cout << "after " << attempts << " attempts still no solution" << std::endl;
    }

    std::cout << "the sequence generated by the code maker was:" << std::endl;
    for(unsigned i = 0; i < maker.sequence.size(); i++){
        std::cout << maker.sequence[i] << " ";
    }

    std::cout << std::endl;
    return 0;
}

void set_random_seed(){
    std::srand(std::time(0));
}

int randn(int n){
    return std::rand() % n;
}

void compa_attem_and_possi_code(int num, const std::vector<int> attempt, const std::vector<int> pool, int& black_hits, int& white_hits){
    black_hits = 0;
    white_hits = 0;
    std::vector<int> pool_copy;
    std::vector<int> attempt_copy;
    int size = attempt.size();

    //copy:
    for(int i = 0; i < size; i++){
       int tmp = pool[i];
       pool_copy.push_back(tmp);
    }

    for(int i = 0; i < size; i++){
       int tmp = attempt[i];
       attempt_copy.push_back(tmp);
    }

    //computing blackhits:
    for(int j = 0; j < size; j++){
       if(attempt_copy[j] == pool_copy[j]){
          black_hits++;
          pool_copy[j] = num;
          attempt_copy[j] = num + 1;
       }
    }

    //computing whitehits:
    if(black_hits != size){
        for(int p = 0; p < size; p++){
           bool found = false;
           for(int q = 0; q < size; q++){
              if(attempt_copy[p] == pool_copy[q] && found == false){
                   white_hits++;
                   pool_copy[q] = num;
                   attempt_copy[p] = num + 1;
                   found = true;
              }
           }
        }
    }

}
