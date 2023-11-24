#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <random>

using namespace std;

using namespace std::chrono;

int baller_k = 0;
int batsmen_k = 0;
int randNum;
auto three_sec = 3s;
int tempary_num;
int total_overs;//number of overs per half

int randomnumber_generator() {
    // Create a random number generator engine
    std::random_device rd; // Seed the engine with a hardware entropy source
    std::mt19937 generator(rd()); // Mersenne Twister engine

    // Define a distribution (e.g., uniform distribution between 1 and 100)
    std::uniform_int_distribution<int> distribution(1, 100);

    
    int random_number = distribution(generator);

    return random_number;

}

class player {
public:
    int battingorder;
    float battingsuccessrate;
    float ballingsuccessrate;
    string playername;
    int playerage;
    int playerscore = 0;//player indivi
    int playerballs = 0;//Balls delivered as a bawler.DOn't include wide balls
    int facedballs = 0;//player faced balls as an batsman
    int ballscore = 0;// Score the bawler gave to oppsite team by balling and wide balls
    int ballerwickets = 0;//total wickets got by baller

    char ballingarm;// L for left arm R for right arm
    char ballingtype;// F for fast S for slow

    player* nextbatsman;
    player* nextballer;


    string getplayername() {
        return playername;
    }

    player(string Playername, int Playerage, player* next_batsman, player* next_baller, float battingsuccessrate, float ballingsuccessrate, int battingorder) {
        this->playername = Playername;
        this->playerage = Playerage;
        this->nextbatsman = next_batsman;
        this->nextballer = next_baller;
        this->ballingsuccessrate = ballingsuccessrate;
        this->battingsuccessrate = battingsuccessrate;
        this->battingorder = battingorder;
    }
    ~player() {

    }

};

class team {
public:
    string teamname;
    int totalruns;
    team* next;
    player* header;

    team(string teamname, team* next) {
        this->teamname = teamname;
        this->next = next;
        this->header = new player("sds", 0, nullptr, nullptr, 0, 0, 0);
    }
    ~team() {}

    void addplayer(string name, float batrate, float ballrate, int battingorder) {
        if (header->getplayername() == "sds") {
            player* newplayer = new player(name, 12, nullptr, nullptr, batrate, ballrate, battingorder);
            header = newplayer;
        }
        else if (header->nextbatsman == nullptr) {
            player* newplayer = new player(name, 12, nullptr, header, batrate, ballrate, battingorder);
            header->nextbatsman = newplayer;
        }
        else {
            player* temphead = header;
            while (temphead->nextbatsman != nullptr) {
                temphead = temphead->nextbatsman;
            }
            player* newplayer = new player(name, 12, nullptr, temphead, batrate, ballrate, battingorder);
            temphead->nextbatsman = newplayer;
        }
    }

    void viewplayers() {
        player* headplayer = header;
        while (headplayer != nullptr) {
            std::cout << headplayer->playername << endl;
            headplayer = headplayer->nextbatsman;
        }
    }
};

class comment;

class scoreboard {
private:
    int score = 0;
    int balls = 0;
    int overs = 0;
    int wickets = 0;
    int total_balls = 0;
    int total_balls_for_over = 0;//total balls sent including wide balls
    team* team1 = new team("", nullptr);
    team* team2;
    player* batsman1;
    player* batsman2;
    player* baller;
    string cur_over_runs = "                                       ";
    team* winner;
public:
    scoreboard(team* team1, team* team2) {
        this->team1 = team1;
        this->team2 = team2;
        batsman1 = team1->header;
        batsman2 = team1->header->nextbatsman;
        player* temphead = team2->header;
        while (temphead->nextbatsman != nullptr) {
            temphead = temphead->nextbatsman;
        }
        baller = temphead;
    }
    ~scoreboard() {}

    int returnballs() {
        return balls;
    }

    void getnextbatsman(player* outedbatman) {
        
        if (outedbatman->battingorder > batsman2->battingorder) {
            batsman1 = outedbatman->nextbatsman;
        }
        else {
            batsman1 = batsman2->nextbatsman;
        }
    }

    void getnextballer() {
        if (baller->nextballer == nullptr) {
            player* temphead2 = team2->header;
            while (temphead2->nextbatsman != nullptr) {
                temphead2 = temphead2->nextbatsman;
            }
            baller = temphead2;
            delete temphead2;
        }
        else {
            baller = baller->nextballer;
        }
    }

    void showbatsman() {
        std::cout << "Batsman 1 is " << batsman1->playername << " & batsman 2 is " << batsman2->playername << endl;
    }
    void showballer() {
        std::cout << "Now ballin " << baller->playername << endl;
    }

    void swapbatsmen() {
        player* temp = batsman1;
        batsman1 = batsman2;
        batsman2 = temp;
    }

    void endoffirsthalf() {
        team* temp = team1;
        team1 = team2;
        team2 = temp;
        batsman1 = team1->header;
        batsman2 = team1->header->nextbatsman;

        player* temphead = team2->header;
        while (temphead->nextbatsman != nullptr) {
            temphead = temphead->nextbatsman;
        }
        baller = temphead;
        balls = 0;
        score = 0;
        overs = 0;
        wickets = 0;

    }

    friend void showscreen(scoreboard* sb, comment* commentar,int);
    friend int OSsystem(scoreboard* score_board, comment* commentar);
    friend class comment;
};

class comment {
    scoreboard* sb;
public:
    string cur_comment = " ";

    comment(scoreboard* x) {
        this->sb = x;
        cur_comment = " ";
    }

    void addcomment(int x) {
        switch (x)
        {
        case 1:
            // code for baller running 
            if (sb->total_balls_for_over == 0) {
          
                cur_comment += "First ball of the over. Now balling "+ sb->baller->playername+" .brilliant ";
                if (sb->baller->ballingarm == 'L') {
                    if (sb->baller->ballingtype == 'F') {
                        cur_comment += "left arm fast baller. With a average success rate of " + to_string(sb->baller->ballingsuccessrate) + ".";
                    }
                    else {
                        cur_comment += "left arm spinner with a average success rate of " + to_string(sb->baller->ballingsuccessrate) + ".";
                    }
                }
                else {
                    if (sb->baller->ballingtype == 'F') {
                        cur_comment += "Right arm fast baller. With a average success rate of " + to_string(sb->baller->ballingsuccessrate) + ".";
                    }
                    else {
                        cur_comment += "Right arm spinner with a average success rate of " + to_string(sb->baller->ballingsuccessrate) + ".";
                    }
                }
            }
            else if (sb->returnballs() == 5) {
                cur_comment += "Bowler looking dedicated as he runs towards balling his final delivery!!........";
            }
            randNum = randomnumber_generator() % 4 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "A smooth rundown towards the popping crease by the bowler..";
                break;
            case 2:
                cur_comment += "Bowler looking committed as he running towards to batsman to his next ball..";
                break;
            case 3:
                cur_comment += "Bawler looks to the heavens as he comes flashing towards the popping crease.Let's see this ball!!...";
                break;
            default:
                cur_comment += "Slight change in the bowling action. Bawler tries something new with fielding format..Let's see what this ball is about...";
                break;
            }
            break;

        case 2:
            //code for a sixer
            tempary_num = sb->total_balls_for_over;
            tempary_num = (tempary_num - 1) * 2;
            sb->cur_over_runs[tempary_num] = '6';
            randNum = randomnumber_generator() % 4 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "Ooohhh!!! He hit this one mile. Great shot. Bigeeeeee.... Brilliant sixer  by the batsman.. what a player!!! A SIXER!!!!";
                break;
            case 2:
                cur_comment += "oohh this is high!!! What a six!! what a SIX !! Its  on the roof.... I think that is the biggest six of the series!.";
                break;
            case 3:
                cur_comment += "Oohh what a magnificant shot!!! That was right out of middle of field and  it's gone sailing in to the crowd.. Another sixer for team " + sb->team1->teamname;
                break;
            default:
                cur_comment += "What a beautiful sixer. Flew right over my head!! ha ha!! that one was a thriller...";
                break;
            }
            break;
        case 3:
            //code for a bounndary
            tempary_num = sb->total_balls_for_over;
            tempary_num = (tempary_num - 1) * 2;
            sb->cur_over_runs[tempary_num] = '4';
            randNum = randomnumber_generator() % 7 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "Ohh i think this is a four.... YES!! It is a beautiful FOUR!!!!! the bowler made a mistake by sending a bad ball, and the batsman took full advantage with a strong hit. The ball raced to the boundary, and everyone could see it coming. FOUR runs added to the scoreboard..";
                break;
            case 2:
                cur_comment += "A beautiful Boundary!!The batsman hit the ball really hard over the fielders' heads for a big score. It's like hitting a home run in baseball, and the crowd loving it!! 4 runs to team "+ sb->team1->teamname;
                break;
            case 3:
                cur_comment += "The batsman gently guided the ball past the fielders and into the open space aaaaaaand... it is a FOUR!!!  It wasn't a powerful shot, but it found the boundary due to smart placement.Another boundary for team " + sb->team1->teamname;
                break;
            case 4:
                cur_comment += "Another 4...The batsman used his feet well to move into position and hit the ball cleanly. It went all the way to the boundary . 4 runs for team " + sb->team1->teamname;
                break;
            case 5:
                cur_comment += "The batsman played a nice sweep shot along the ground. The ball is racing to the boundary , fielder is running after it.. Will he be able to catch it..(moment of silence) No!! that is a close boundary!! Nice try by the fielder. boundary for team " + sb->team1->teamname;
                break;
            case 6:
                cur_comment += "The bowler tried to surprise the batsman with a fast ball, but he was ready. He quickly flicked the ball away for a boundary.Another boundary for team " + sb->team1->teamname;
                break;
            default:
                cur_comment += "The batsman was under pressure, but he remained calm. He pulled the ball away with precision, and it sailed to the boundary.";
                break;
            }
            break;
        case 4:
            //code for single run
            tempary_num = sb->total_balls_for_over;
            tempary_num = (tempary_num - 1) * 2;
            sb->cur_over_runs[tempary_num] = '1';
            randNum = randomnumber_generator() % 8 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "Wow, what hustle from the batsmen! They sprinted like lightning between the wickets, turning a single into a heart-pounding double. The fielders scrambled, but the batsmen's lightning speed and smart running left everyone on the edge of their seats. The crowd is roaring with excitement! A close run from the batsman!!";
                break;
            case 2:
                cur_comment += "A mighty swing of the bat! The ball sails high into the air, but it's safely caught by the fielder in the deep. A brave effort from the batsman, and they sneak in a single.";
                break;
            case 3:
                cur_comment += "A solid forward defensive stroke from the batsman, textbook perfection. The ball gently rolls along the ground, finding its way to the fielder, and the batsmen seize the opportunity for a quick single.";
                break;
            case 4:
                cur_comment += "The batsman rocks back and cuts the ball square of the wicket. The fielder gives chase, but it's a lost cause as the ball races away for a run. They're rotating the strike well. Single run for " + sb->team1->teamname;
                break;
            case 5:
                cur_comment += "A powerful pull shot! The batsman rocks onto the back foot and sends the ball soaring to the backline. What a shot!! Pure technique!! One more run...";
                break;
            case 6:
                cur_comment += "He leans into the drive and sends the ball straight down the ground. It's a glorious sight as the ball races away .they run a quick single.";
                break;
            case 7:
                cur_comment += "A deft touch by the batsman! He guides the ball delicately down to the third man region, and it's a safe single. Good running between the wickets.";
                break;
            default:
                cur_comment += "A late cut from the batsman! The ball kisses the edge of the bat and runs past the wicketkeeper for a single. Smart batting to rotate the strike.";
                break;
            }
            break;
        case 5:
            //code for double run
            tempary_num = sb->total_balls_for_over;
            tempary_num = (tempary_num - 1) * 2;
            sb->cur_over_runs[tempary_num] = '2';

            randNum = randomnumber_generator() % 7 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "In the blink of an eye, they've taken two runs! The fielders are left bewildered as these batsmen put their running shoes on and show us how it's done. Two more runs added to the scoreboard!!!";
                break;
            case 2:
                cur_comment += "The batsmen are like a well-oiled machine out there! They've taken off like a rocket, and the fielders can't catch them. Two runs added to the total, and the crowd is loving it! Two runs for team " + sb->team1->teamname;
                break;
            case 3:
                cur_comment += "What a fantastic display of running between the wickets! These two are turning singles into doubles with ease. It's like a relay race out there, and they're winning it! Two runs for team " + sb->team1->teamname;
                break;
            case 4:
                cur_comment += "The batsmen are in perfect sync, dashing between the stumps. They've got the fielders on their toes, and they're squeezing every run they can. It's cricket at its electrifying best! Two runs for team " + sb->team1->teamname;
                break;
            case 5:
                cur_comment += "The batsmen's communication is spot-on! They're pushing each other to the limit, racing back for the second run. This is edge-of-the-seat stuff, folks! Two runs for team " + sb->team1->teamname;
                break;
            case 6:
                cur_comment += "The fielders must be feeling the heat as these two run like there's no tomorrow! They've just turned a dot ball into a double, and the pressure is mounting. Two runs for team " + sb->team1->teamname;
                break;
            default:
                cur_comment += "It's like a race out there, and the batsmen are winning it every time! The scoreboard keeps ticking, thanks to their incredible running between the wickets. What a spectacle! Brilliantly earned two runs for the team...";
                break;
            }
            break;
        case 6:
            //code for wicket
            tempary_num = sb->total_balls_for_over;
            tempary_num = (tempary_num - 1) * 2;
            sb->cur_over_runs[tempary_num] = 'W';

            randNum = randomnumber_generator() % 7 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "Ohhhhhh!!!! What a ball!!! Brilliant direct delivery to the middle stump by " + sb->baller->playername + " . What a savage!!! .. Next batsman coming to the field.";
                break;
            case 2:
                cur_comment += "BOWLED!! There's NO OTHER WORDS other than BOWLED.. He's been done by the pace! The ball swung back in and hit the middle stump!! Beautiful delivery....";
                break;
            case 3:
                cur_comment += sb->batsman1->playername + " went for a duck but edged it. Ball is going higher & higher..Let's see if they manage to catch it.... Oh!! They caught it. Beautiful catch by the fielder. And a big mistake from batsman!!";
                break;
            case 4:
                cur_comment += "The batsmen are going for a run ..Oh!!!!! Quick action by the slip keeper. Will they manage to take the runout?? Nicely earned runout.. Umpire is showing the finger. And that is a runout gentlemen...Another wicket earned by team " + sb->team2->teamname;
                break;
            case 5:
                cur_comment += "LBW!! Umpire's finger has gone up! He's been adjudged out lbw!! Let's see the third umpire reviews.......Yup.That is a LBW. Clean wicket. The ball was hitting the stumps!! Good ball by" + sb->baller->playername;
                break;
            case 6:
                cur_comment += "Ohhh!! Ball just went straight up in the air!!! I think this might be it...... Very well caught by the keeper. ANother batsman lost to team " + sb->team1->teamname;
                break;
            default:
                cur_comment += "Stumped! He's been stumped! Batsman was out of his crease and the wicketkeeper has taken the bails off!! Nice action by wicket keeper!!";
                break;
            }
            break;
        case 7:
            //code for wide ball
            tempary_num = sb->total_balls_for_over;
            tempary_num = (tempary_num - 1) * 2;
            sb->cur_over_runs[tempary_num] = 'w';
            sb->cur_over_runs[tempary_num + 1] = 'd';
            sb->total_balls_for_over++;
            randNum = randomnumber_generator() % 3 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "That's a wide! The bowler has strayed down the leg side and the umpire has called it wide.";
                break;
            case 2:
                cur_comment += "That's a poor delivery from the bowler. He needs to tighten his line and length. Umpire signals a wide ball... Free point to " + sb->team1->teamname;
                break;
            case 3:
                cur_comment += "That's a wide! The bowler has bowled a ball so wide, it's almost gone into the crowd. The batsman is probably laughing to himself!";
                break;
            default:
                cur_comment += "That's a costly wide for the bowling team! It's given the batsman an extra run and it's also put the bowler off his rhythm.";
                break;
            }
            break;
        case 8:
            //comment for first half
            cur_comment += "Good morning everyone!!! Welcome to cricket finals between " + sb->team1->teamname + " and " + sb->team2->teamname + " . " + sb->team1->teamname + " has won the toss and selected to bat first. Players are entering to the field. " + sb->batsman1->playername + " with " + to_string(sb->batsman1->battingsuccessrate) + " success rate over past 40+ matches and ";
            cur_comment += sb->batsman2->playername + " with a success rate of " + to_string(sb->batsman2->battingsuccessrate) + " over 50+ matches will be joining the first over as open batsman. Let's see the first delivery of the match.. First over will be delivered by " + sb->baller->playername;
            break;
        case 9:
            //comment for second half
            cur_comment += "Welcome to second half of the cricket finals between " + sb->team2->teamname + " and " + sb->team2->teamname + ". In the first half " + sb->team2->teamname + " scored " + to_string(sb->team2->totalruns) + "players are entering back to the field. " + sb->batsman1->playername + " with " + to_string(sb->batsman1->battingsuccessrate) + " success rate over past 40+ matches and ";
            cur_comment += sb->batsman2->playername + " with a success rate of " + to_string(sb->batsman2->battingsuccessrate) + " over 50+ matches will be joining the first over as open batsman. Let's see the first delivery of the match.. First over will be delivered by " + sb->baller->playername;
            break;

        case 10:
            //comment for win of second team
            cur_comment += "Oh!! And that is it. Team " + sb->team1->teamname + " have chase the victory . Scored " + to_string(sb->score) + " runs from " + to_string(sb->overs) + " overs & " + to_string(sb->balls) + " balls. Truly beautiful second half. Well done champions!!. Team " + sb->team2->teamname + " , Well played!!. Brilliant performance by both team. And thats the end of the match. I'm Sahas vinandith. See you in the next match. good night!!";
            break;
        case 11:
            //comment for  win of first team
            cur_comment += "Oh!! And that is the end of team " + sb->team1->teamname + " . team " + sb->team2->teamname + " sealed the hope for victory of " + sb->team1->teamname + " with that wicket . Brilliant play by both teams. Team " + sb->team2->teamname + " have prove themselves as the champions. Good luck to both teams.. AND thats the end of the match.. I'm your friendly commentater \"sahas vinandith\". And i'll see you on the next match..Good night!!";
            break;
        case 12:
            //comment for end of an over
            sb->cur_over_runs= "                                       ";
            cur_comment += "thats the end of this over. total overs are " + to_string(sb->overs) + " & total runs are " + to_string(sb->score) + " . Next balling " + sb->baller->playername + " . Fielder are changing the sides. The two batssmen are discussing in the middle of the field. Possibly a good strategy...";
            break;
        case 13:
            //comment for end of first half
            cur_comment += " That is the end of the first half. Team " + sb->team1->teamname + " scored " + to_string(sb->score) + " with " + to_string(sb->wickets) + ". See you after 15 minuts in the second half...";
            break;
        case 14:
            //comment for end of second half (which means second bat team lost)
            cur_comment += "Ok thats the end of team " + sb->team1->teamname + " . Team " + sb->team2->teamname + " did  a good job with fielding and will leave the field as the champions of this series. Good perfomance by both of teams.. Congratulations champions... I'm your friendly host \"sahas vinandith\" and i'll see you inn the next match..";
            break;
        case 15:
            //comment for no runs
            tempary_num = sb->total_balls_for_over;
            tempary_num = (tempary_num - 1) * 2;
            sb->cur_over_runs[tempary_num] = '0';
            randNum = randomnumber_generator() % 5 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "An immaculate delivery, beating the bat with surgical precision. The bowler is really testing the batsman's patience and technique here. No runs in this delivery...";
                break;
            case 2:
                cur_comment += "A dot ball, as the batsman plays it back to the bowler. Good delivery from the bowler......";
                break;
            case 3:
                cur_comment += "The bowler keeps it tight, no run opportunity for the batsman. Overs are ending.. Batsmen feeling the pressure....";
                break;
            case 4:
                cur_comment += "Batsmen are running.. Oh they went back to their positions.. these fielders are doing impressive job today... Balls are ending... Batsmen feel the pressure..";
                break;
            default:
                cur_comment += "The fielders are in close, no chance for a single here. No runs in this delivery....";
                break;
            }
            cur_comment += "An immaculate delivery, beating the bat with surgical precision. The bowler is really testing the batsman's patience and technique here. No runs in this delivery...";
            break;
        default:
            break;
        }
    }

};

void showscreen(scoreboard* sb, comment* commentar,int turn=1) {
    if (turn == 1) {
        std::cout << "\n\t\tBatting: " << sb->team1->teamname << "\t\t\t\t\t\t\t\t\t\t  Balling: " << sb->team2->teamname << endl;
        std::cout << "\t\t" << sb->team1->teamname << "-  " << sb->score << "/" << sb->wickets << "(" << sb->overs << "." << sb->balls << ')' << "  |  " << sb->batsman1->playername << "-" << sb->batsman1->playerscore << "(" << sb->batsman1->facedballs << ")*"
            << "\t| " << sb->batsman2->playername << "-" << sb->batsman2->playerscore << "(" << sb->batsman2->facedballs << ")\t | " +sb->cur_over_runs+"    |   " << sb->baller->playername << "-" << sb->baller->ballerwickets << "/" << sb->baller->ballscore << "(" << sb->baller->playerballs << ")\n";
        std::cout << "\n******************************************************************************************************************************************************************\n"
            "Comment:-\n" << commentar->cur_comment;
    }
    else if(turn==2){
        int temp_rand = randomnumber_generator() % 2;
        if (temp_rand == 0) {

            std::cout << "\n\t\tBatting: " << sb->team1->teamname << "\t\t\t\t\t\t\t\t\t\t  Balling: " << sb->team2->teamname << endl;
            std::cout << "\t\t" << sb->team1->teamname << "-  " << sb->score << "/" << sb->wickets << "(" << sb->overs << "." << sb->balls << ')' << "  |  " << sb->batsman1->playername << "-" << sb->batsman1->playerscore << "(" << sb->batsman1->facedballs << ")*"
                << "\t| " << sb->batsman2->playername << "-" << sb->batsman2->playerscore << "(" << sb->batsman2->facedballs << ")\t\tNeed " + to_string((sb->team2->totalruns) - (sb->score)) + " runs in " + to_string(total_overs * 6 - (sb->total_balls)) + " balls.\t   " + "  |   " << sb->baller->playername << "-" << sb->baller->ballerwickets << "/" << sb->baller->ballscore << "(" << sb->baller->playerballs << ")\n";
            std::cout << "\n******************************************************************************************************************************************************************\n";
            std::cout << "\nComment:-\n" << commentar->cur_comment;
        }
        else {
            std::cout << "\n\t\tBatting: " << sb->team1->teamname << "\t\t\t\t\t\t\t\t\t\t  Balling: " << sb->team2->teamname << endl;
            std::cout << "\t\t" << sb->team1->teamname << "-  " << sb->score << "/" << sb->wickets << "(" << sb->overs << "." << sb->balls << ')' << "  |  " << sb->batsman1->playername << "-" << sb->batsman1->playerscore << "(" << sb->batsman1->facedballs << ")*"
                << "\t| " << sb->batsman2->playername << "-" << sb->batsman2->playerscore << "(" << sb->batsman2->facedballs << ")\t | " + sb->cur_over_runs + "   |   " << sb->baller->playername << "-" << sb->baller->ballerwickets << "/" << sb->baller->ballscore << "(" << sb->baller->playerballs << ")\n";
            std::cout << "\n******************************************************************************************************************************************************************\n"
                "\nComment:-\n" << commentar->cur_comment;
        }
    }
    else if(turn==3){// half summary
        std::cout << "\n\t\t\t\t\t\t\033[7m\033[1mSummary of first half\033[0m\033[30mj \033[0m\n";
        std::cout << "\n\t\t\t    "<< "\033[1m" <<sb->team1->teamname<< "\t\t\t\t\t "<< sb->team2->teamname<<"\033[0m"<<endl;//first line
        std::cout << "\n\t\t\033[1m" << "Batsmen " << "    stat     \033[35G   " << " Score"<< "-Balls " << " \t\t      Bawler\t\t  Wickts-Balls(Score)\033[0m";
       
        player* tempbatsman=sb->team1->header;
        while (tempbatsman != sb->batsman1 && tempbatsman != sb->batsman2) {
           std::cout << "\n\t\t\033[1m" << tempbatsman->playername << " \033[0m  \033[29G\033[7m   OUT   \033[0m  \033[40G\033[7m "<<to_string(tempbatsman->playerscore)<<"  "<<to_string(tempbatsman->facedballs)<<" \033[0m\n";
           tempbatsman = tempbatsman->nextbatsman;
        }
        
        std::cout << "\n\t\t\033[1m" << sb->batsman1->playername << " \033[0m  \033[29G\033[7m NOT OUT \033[0m  \033[40G\033[7m " << to_string(sb->batsman1->playerscore) << "  " << to_string(sb->batsman1->facedballs) << " \033[0m \033[30mj \033[0m\n";
        std::cout << "\n\t\t\033[1m" << sb->batsman2->playername << " \033[0m  \033[29G\033[7m NOT OUT \033[0m  \033[40G\033[7m " << to_string(sb->batsman2->playerscore) << "  " << to_string(sb->batsman2->facedballs) << " \033[0m \033[30mj \033[0m\n";
        
        if (tempbatsman == sb->batsman2) {
            tempbatsman = sb->batsman2->nextbatsman;
            while (tempbatsman != sb->batsman1 ) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << " \033[0m  \033[29G\033[7m   OUT   \033[0m  \033[40G\033[7m " << to_string(tempbatsman->playerscore) << "  " << to_string(tempbatsman->facedballs) << " \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;
                
            }
            tempbatsman = tempbatsman->nextbatsman;
            while (tempbatsman != nullptr) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << "\033[0m  \033[7m\033[29G         \033[0m  \033[40G\033[7m      \033[0m \033[0m \033[30mj \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;
            }
        }
        else {
            tempbatsman = sb->batsman1->nextbatsman;
            while (tempbatsman != sb->batsman2) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << " \033[0m  \033[29G\033[7m   OUT  \033[0m  \033[40G\033[7m " << to_string(tempbatsman->playerscore) << "  " << to_string(tempbatsman->facedballs) << " \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;
            }
            tempbatsman = tempbatsman->nextbatsman;
            while (tempbatsman != nullptr) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << "\033[0m  \033[G\033[7m\033[29G         \033[0m  \033[40G\033[7m      \033[0m \033[0m \033[30mj \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;
            }
        }
        player* tempballer = sb->team2->header;
        
        std::cout << "\033[A";
        for (int tem = 0; tem < 11; tem++) {
            std::cout << "\033[70G \033[1m"<<tempballer->playername<<"\033[0m  \033[99G\033[7m"<<tempballer->ballerwickets<<" - "<<tempballer->playerballs<<" ("<<tempballer->ballscore<<")\033[0m \033[30mj \033[0m\033[A\033[A";
            tempballer = tempballer->nextbatsman;
        }
        for (int i = 0; i < 11; i++) {
            cout << "\n\n";
        }
        cout << "\n\n\t\t\033[1m\033[33mTotal \033[0m\033[40G" << sb->team1->totalruns<<endl<<endl;

    }
    else //output second half
    {
        std::cout << "\n\t\t\t\t\t\t\033[7m\033[1mSummary of first half\033[0m\033[30mj \033[0m\n";
        std::cout << "\n\t\t\t    " << "\033[1m" << sb->team1->teamname << "\t\t\t\t\t " << sb->team2->teamname << "\033[0m" << endl;//first line
        std::cout << "\n\t\t\033[1m" << "Batsmen " << "    stat     \033[35G   " << " Score" << "-Balls " << " \t\t      Bawler\t\t  Wickts-Balls(Score)\033[0m";

        player* tempbatsman = sb->team1->header;
        while (tempbatsman != sb->batsman1 && tempbatsman != sb->batsman2) {
            std::cout << "\n\t\t\033[1m" << tempbatsman->playername << " \033[0m  \033[29G\033[7m   OUT   \033[0m  \033[40G\033[7m " << to_string(tempbatsman->playerscore) << "  " << to_string(tempbatsman->facedballs) << " \033[0m\n";
            tempbatsman = tempbatsman->nextbatsman;
        }

        std::cout << "\n\t\t\033[1m" << sb->batsman1->playername << " \033[0m  \033[29G\033[7m NOT OUT \033[0m  \033[40G\033[7m " << to_string(sb->batsman1->playerscore) << "  " << to_string(sb->batsman1->facedballs) << " \033[0m \033[30mj \033[0m\n";
        std::cout << "\n\t\t\033[1m" << sb->batsman2->playername << " \033[0m  \033[29G\033[7m NOT OUT \033[0m  \033[40G\033[7m " << to_string(sb->batsman2->playerscore) << "  " << to_string(sb->batsman2->facedballs) << " \033[0m \033[30mj \033[0m\n";

        if (tempbatsman == sb->batsman2) {
            tempbatsman = sb->batsman2->nextbatsman;
            while (tempbatsman != sb->batsman1) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << " \033[0m  \033[29G\033[7m   OUT   \033[0m  \033[40G\033[7m " << to_string(tempbatsman->playerscore) << "  " << to_string(tempbatsman->facedballs) << " \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;

            }
            tempbatsman = tempbatsman->nextbatsman;
            while (tempbatsman != nullptr) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << "\033[0m  \033[7m\033[29G         \033[0m  \033[40G\033[7m      \033[0m \033[0m \033[30mj \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;
            }
        }
        else {
            tempbatsman = sb->batsman1->nextbatsman;
            while (tempbatsman != sb->batsman2) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << " \033[0m  \033[29G\033[7m   OUT  \033[0m  \033[40G\033[7m " << to_string(tempbatsman->playerscore) << "  " << to_string(tempbatsman->facedballs) << " \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;
            }
            tempbatsman = tempbatsman->nextbatsman;
            while (tempbatsman != nullptr) {
                std::cout << "\n\t\t\033[1m" << tempbatsman->playername << "\033[0m  \033[G\033[7m\033[29G         \033[0m  \033[40G\033[7m      \033[0m \033[0m \033[30mj \033[0m\n";
                tempbatsman = tempbatsman->nextbatsman;
            }
        }
        
        player* tempballer = sb->team2->header;

        std::cout << "\033[A";
        for (int tem = 0; tem < 11; tem++) {
            std::cout << "\033[70G \033[1m" << tempballer->playername << "\033[0m  \033[99G\033[7m" << tempballer->ballerwickets << " - " << tempballer->playerballs << " (" << tempballer->ballscore << ")\033[0m \033[30mj \033[0m\033[A\033[A";
            tempballer = tempballer->nextbatsman;
        }
        for (int i = 0; i < 11; i++) {
            cout << "\n\n";
        }
        cout << "\n\n\t\t\033[1m\033[33mTotal \033[0m\033[40G" << sb->team1->totalruns<<endl;
        cout << "\n";
        std::cout << "\n\n\t\t\t\t\033[1m\033[32m WINNERS: \033[37m" << sb->winner->teamname << endl<<endl;
    }
}

class cricketmatch {
public:
    team* team2;
    team* team1;
    
    cricketmatch() {
        team2 = new team("0", nullptr);
        team1 = new team("0", team2);
    }
};

void screenrefresh(scoreboard* SCB, comment* commentar,int x=1) {
    system("cls");
    showscreen(SCB, commentar,x);
    this_thread::sleep_for(2s);
    //system("cls");
}

void textprep(string textline, cricketmatch& match) {
    string up = textline;
    transform(up.begin(), up.end(), up.begin(), ::toupper);
    string command, team_name, player_name;
    int age, battingorder,number_of_overs;
    float batsuccess, ballsuccess;
    stringstream geek(up);
    geek >> command;
    if (command == "CREATE") {
        geek>> team_name;
        if (match.team1->teamname == "0") {
            match.team1->teamname = team_name;
        }
        else {
            match.team2->teamname = team_name;
        }
    }
    else if (command == "ADD") {
        geek >> team_name>>player_name >> age >> batsuccess >> ballsuccess >> battingorder;

        team* tempteam=new team("kvi",nullptr);
        if (team_name == match.team1->teamname) {
            tempteam = match.team1;
        }
        else if (team_name == match.team2->teamname) {
            tempteam = match.team2;
        }
        tempteam->addplayer(player_name, batsuccess, ballsuccess, battingorder);
    }
    else if (command == "MATCH") {
        geek >> number_of_overs;
        total_overs = number_of_overs;
    }
}

int OSsystem(scoreboard* score_board, comment* commentar) {
    using namespace std;
    int win = 0;
    for (int k = 0; k < 2; k++) {
        int half_finish = 0;//indicate if an half is over (by going all the wickets or all the overs
        if (k == 0) {
            commentar->addcomment(8);
            screenrefresh(score_board, commentar);
            this_thread::sleep_for(2s);
            commentar->cur_comment = " ";

        }
        else {
            commentar->addcomment(9);
            screenrefresh(score_board, commentar);
            this_thread::sleep_for(2s);
            commentar->cur_comment = " ";
        }
        while (score_board->overs != total_overs) {
            for (int i = 0; i < 6; i++) {

                commentar->addcomment(1);//comment for baller start running
                screenrefresh(score_board, commentar,k+1);
                baller_k = randomnumber_generator() % 100 + 1;
                batsmen_k = randomnumber_generator() % 100 + 1;
                (score_board->total_balls_for_over)++;
                if (baller_k<int(score_board->baller->ballingsuccessrate)) {//ball is a good ball
                    (score_board->balls)++;
                    (score_board->total_balls)++;
                    (score_board->batsman1->facedballs)++;
                    (score_board->baller->playerballs)++;

                    if ((batsmen_k*(score_board->batsman1->battingsuccessrate)/100)<(baller_k*(score_board->baller->ballingsuccessrate)/100 + 5) && (batsmen_k * (score_board->batsman1->battingsuccessrate)/100) > (baller_k * (score_board->baller->ballingsuccessrate)/100- 5)) { // check if it is a out ball
                        //outputof batsmen out
                        
                        (score_board->baller->ballerwickets)++;
                        (score_board->wickets)++;
                        
                        if (k == 0) {
                            commentar->addcomment(6);
                            screenrefresh(score_board, commentar);
                        }
                        else {
                            commentar->addcomment(6);
                            screenrefresh(score_board, commentar, 2);
                        }
                        
                        score_board->getnextbatsman(score_board->batsman1);
                        
                        
                        if (score_board->wickets == 10) {//all out situationn
                            half_finish = 1;
                            
                            if (k == 1) {
                                win = 1;
                                commentar->cur_comment = " ";
                                commentar->addcomment(11);
                                screenrefresh(score_board, commentar);
                                this_thread::sleep_for(3s);
                                commentar->cur_comment = " ";
                                score_board->winner = score_board->team2;//winner is the balling team (team 2)
                            }
                            break;
                        }
                    }
                    else if (batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.1)) {
                        //output sixer
                        score_board->team1->totalruns +=6 ;
                        (score_board->score) += 6;
                        (score_board->batsman1->playerscore) += 6;
                        (score_board->baller->ballscore) += 6;
                       
                        if (k == 0) {
                            commentar->addcomment(2);
                            screenrefresh(score_board, commentar);
                        }
                        else {
                            commentar->addcomment(2);
                            screenrefresh(score_board, commentar, 2);
                        }
                       
                       

                    }
                    else if (batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.3)) {
                        //output for boundary
                        score_board->team1->totalruns +=4 ;
                        (score_board->score) += 4;
                        (score_board->batsman1->playerscore) += 4;
                        (score_board->baller->ballscore) += 4;
                        if (k == 0) {
                            commentar->addcomment(3);
                            screenrefresh(score_board, commentar);
                        }
                        else {
                            commentar->addcomment(3);
                            screenrefresh(score_board, commentar, 2);
                        }
                        

                    }
                
                    else if (batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.6)) {
                        //output double run
                        score_board->team1->totalruns +=2 ;
                        (score_board->score) += 2;
                        (score_board->batsman1->playerscore) += 2;
                        (score_board->baller->ballscore) += 2;
                        if (k == 0) {
                            commentar->addcomment(5);
                            screenrefresh(score_board, commentar);
                        }
                        else {
                            commentar->addcomment(5);
                            screenrefresh(score_board, commentar, 2);
                        }
                        


                    }
                    else if(batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.8)){
                        //output  run
                        score_board->team1->totalruns +=1 ;
                        (score_board->score)++;
                        (score_board->batsman1->playerscore)++;
                        (score_board->baller->ballscore) += 1;

                        if (k == 0) {
                            commentar->addcomment(4);
                            screenrefresh(score_board, commentar);
                        }
                        else {
                            commentar->addcomment(4);
                            screenrefresh(score_board, commentar, 2);
                        }
                        score_board->swapbatsmen();
                        

                    }
                    else {
                        //output no run
                        if (k == 0) {
                            commentar->addcomment(15);
                            screenrefresh(score_board, commentar);
                        }
                        else {
                            commentar->addcomment(15);
                            screenrefresh(score_board, commentar, 2);
                        }
                    }

                }
                else {
                    //output wide ball
                    score_board->team1->totalruns +=1 ;
                    (score_board->score)++;
                    (score_board->baller->ballscore) += 1;
                    if (k == 0) {
                        commentar->addcomment(7);
                        screenrefresh(score_board, commentar);
                    }
                    else {
                        commentar->addcomment(7);
                        screenrefresh(score_board, commentar, 2);
                    }
                
                    i--;
                }
                
                if (k == 1 && (score_board->score) >= (score_board->team2->totalruns)) {
                    //comment for match over by following (second bats are the winners(team1))
                    commentar->addcomment(10);
                    this_thread::sleep_for(3s);
                    screenrefresh(score_board, commentar);
                    half_finish = 1;
                    win = 1;
                    score_board->winner = score_board->team1;
                    break;
                }
                commentar->cur_comment = "";

            }
            if (half_finish == 1) {
                break;
            }
            //comment for end of over
            
            score_board->overs++;
            score_board->balls = 0;
            score_board->total_balls_for_over = 0;
            score_board->getnextballer();
            score_board->swapbatsmen();

            commentar->cur_comment = " ";
            commentar->addcomment(12);
            screenrefresh(score_board, commentar);
            commentar->cur_comment = " ";
        }
        if (win == 1) {
            system("cls");
            showscreen(score_board, commentar, 4);//match summary for second half
            system("pause");
            break;
        }
        if (k != 1) {//end of first half
            
            score_board->total_balls = 0;
            commentar->cur_comment = " ";
            commentar->addcomment(13);
            screenrefresh(score_board, commentar);
            commentar->cur_comment = " ";

            system("cls");
            showscreen(score_board, commentar, 3);//match summary for first half
            system("pause");

            score_board->endoffirsthalf();
            commentar->cur_comment = " ";
        }
        else {//end of second half (second bat team lost)
            win = 1;
            commentar->cur_comment = " ";
            commentar->addcomment(14);
            screenrefresh(score_board, commentar);
            score_board->winner = score_board->team2;

            system("cls");
            showscreen(score_board, commentar, 4);//match summary for second half
            system("pause");
        }
    }
    return 0;
}

int main() {
    cricketmatch match_SlvsAus;

    fstream myfile;
    myfile.open("inputs.txt", ios::in);
    if (myfile.is_open()) {
        string line;
        while (getline(myfile, line)) {
            textprep(line, match_SlvsAus);
        }
    }
    else {
        std::cout << "File not found";
    }

    scoreboard* scoreboardnew = new scoreboard(match_SlvsAus.team1, match_SlvsAus.team2);
    comment* Sahas = new comment(scoreboardnew);
    OSsystem(scoreboardnew, Sahas);
    /*cout << endl;
    showscreen(scoreboardnew, Sahas, 3);
    cout << endl;*/
    //showscreen(scoreboardnew, Sahas, 1);
   // showscreen(scoreboardnew, Sahas, 2);

    return 0;
}