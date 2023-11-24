#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;

using namespace std::chrono;

int baller_k = 0;
int batsmen_k = 0;
int randNum;
auto three_sec = 3s;

class player {
public:
    int battingorder;
    float battingsuccessrate;
    float ballingsuccessrate;

    string playername;
    int playerage;
    int playerscore = 0;
    int playerballs = 0;
    int facedballs = 0;
    int ballscore = 0;
    int ballerwickets = 0;

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
            cout << headplayer->playername << endl;
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
    team* team1;
    team* team2;
    player* batsman1;
    player* batsman2;
    player* baller;
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
        if (outedbatman == batsman1) {
            batsman1 = batsman2;
            batsman2 = batsman2->nextbatsman;
        }
        else {
            batsman2 = batsman2->nextbatsman;
        }
    }

    void getnextballer() {
        baller = baller->nextballer;
    }

    void showbatsman() {
        cout << "Batsman 1 is " << batsman1->playername << " & batsman 2 is " << batsman2->playername << endl;
    }
    void showballer() {
        cout << "Now ballin " << baller->playername << endl;
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

    friend void showscreen(scoreboard* sb, comment* commentar);
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
            /* code for baller running */
            if (sb->returnballs() == 0) {
                cout << cur_comment;
                cur_comment += "First ball of the over. Now balling  Brilliant ";//+(sb->baller->playername)
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
                cur_comment += "looking dedicated as he runs towards balling his final delivery!!";
            }
            randNum = rand() % 4 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "A smooth rundown towards the popping crease by the bowler..";
                break;
            case 2:
                cur_comment += "Bowler looking committed as he running towards to batsman to his next ball..";
                break;
            case 3:
                cur_comment += "Looks to the heavens as he comes flashing towards the popping crease.Let's see this ball!!..";
                break;
            default:
                cur_comment += "Slight change in the bowling action. Bawler tries something new with fielding format..Let's see what this ball is about...";
                break;
            }
            break;

        case 2:
            //code for a sixer
            randNum = rand() % 2 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "Ooohhh!!! He hit this one mile. Great shot. Bigeeeeee.... Brilliant sixer  by the batsman.. what a player!!!";
                break;
            case 2:
                cur_comment += "oohh this is high!!! What a six!! what a six!! Its  on the roof....";
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
            randNum = rand() % 7 + 1;
            switch (randNum)
            {
            case 1:
                cur_comment += "The bowler made a mistake by sending a bad ball, and the batsman took full advantage with a strong hit. The ball raced to the boundary, and everyone could see it coming.";
                break;
            case 2:
                cur_comment += "The batsman hit the ball really hard over the fielders' heads for a big score. It's like hitting a home run in baseball, and the crowd loved it!";
                break;
            case 3:
                cur_comment += "The batsman gently guided the ball past the fielders and into the open space. It wasn't a powerful shot, but it found the boundary due to smart placement.Another boundary for team " + sb->team1->teamname;
                break;
            case 4:
                cur_comment += "The batsman used his feet well to move into position and hit the ball cleanly. It went all the way over the boundary for a maximum score.Another boundary for team " + sb->team1->teamname;
                break;
            case 5:
                cur_comment += "The batsman played a nice sweep shot along the ground. The ball raced away to the boundary, and the fielders couldn't stop it.Another boundary for team " + sb->team1->teamname;
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
            randNum = rand() % 8 + 1;
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
            randNum = rand() % 7 + 1;
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
        default:
            break;
        }
    }

};

void showscreen(scoreboard* sb, comment* commentar) {
    std::cout << "\nBatting: " << sb->team1->teamname << "\t\t\t\t\t\tBalling: " << sb->team2->teamname << endl;
    std::cout << sb->team1->teamname << "-  " << sb->score << "/" << sb->wickets << "(" << sb->overs << "." << sb->balls << ')' << "  |  " << sb->batsman1->playername << "-" << sb->batsman1->playerscore << "(" << sb->batsman1->facedballs << ")*"
        << "\t| " << sb->batsman2->playername << "-" << sb->batsman2->playerscore << "(" << sb->batsman2->facedballs << ")\t\t   |   " << sb->baller->playername << "-" << sb->baller->ballerwickets << "/" << sb->baller->ballscore << "(" << sb->baller->playerballs << ")\n";
    cout << "\nComment:-\n" << commentar->cur_comment;

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

void screenrefresh(scoreboard* SCB, comment* commentar) {
    system("cls");
    showscreen(SCB, commentar);
    this_thread::sleep_for(5s);
    system("cls");
}

void textprep(string textline, cricketmatch& match) {
    string up = textline;
    transform(up.begin(), up.end(), up.begin(), ::toupper);
    string command, team_name, player_name;
    int age, battingorder;
    float batsuccess, ballsuccess;
    stringstream geek(up);
    geek >> command >> team_name;
    if (command == "CREATE") {
        if (match.team1->teamname == "0") {
            match.team1->teamname = team_name;
        }
        else {
            match.team2->teamname = team_name;
        }
    }
    else if (command == "ADD") {
        geek >> player_name >> age >> batsuccess >> ballsuccess >> battingorder;

        team* tempteam=new team("",nullptr);
        if (team_name == match.team1->teamname) {
            tempteam = match.team1;
        }
        else if (team_name == match.team2->teamname) {
            tempteam = match.team2;
        }
        tempteam->addplayer(player_name, batsuccess, ballsuccess, battingorder);
    }
}

int OSsystem(scoreboard* score_board, comment* commentar) {
    for (int k = 0; k < 2; k++) {
        while (score_board->overs != 10) {
            for (int i = 0; i < 6; i++) {
                // cout<<"\n*****"<<commentar->cur_comment<<"***********************"<<endl;
                // this_thread::sleep_for(2s);
                commentar->addcomment(1);
                screenrefresh(score_board, commentar);
                baller_k = rand() % 100 + 1;
                batsmen_k = rand() % 100 + 1;
                if (baller_k<int(score_board->baller->ballingsuccessrate)) {
                    (score_board->balls)++;
                    (score_board->batsman1->facedballs)++;
                    (score_board->baller->playerballs)++;
                    if (batsmen_k<baller_k + 5 && batsmen_k>baller_k - 5) {
                        //outputof batsmen out
                        (score_board->baller->ballerwickets)++;
                        (score_board->wickets)++;
                        cout << score_board->batsman1->playername << " is out.";
                        score_board->getnextbatsman(score_board->batsman1);
                        cout << "the next batsman is" << score_board->batsman2->playername << endl;//should be commented
                        screenrefresh(score_board, commentar);
                    }
                    else if (batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.1)) {
                        //output sixer
                        cout << " Beautyful sixer by " << score_board->batsman1->playername << endl;
                        (score_board->score) += 6;
                        (score_board->batsman1->playerscore) += 6;
                        (score_board->baller->ballscore) += 6;
                        commentar->addcomment(2);
                        screenrefresh(score_board, commentar);

                    }
                    else if (batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.2)) {
                        //output 4
                        cout << " Beautyful 4 by " << score_board->batsman1->playername << endl;
                        (score_board->score) += 4;
                        (score_board->batsman1->playerscore) += 4;
                        (score_board->baller->ballscore) += 4;
                        commentar->addcomment(3);
                        screenrefresh(score_board, commentar);

                    }
                    else if (batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.3)) {
                        //output triple run
                        cout << " Brilliant triple run by " << score_board->batsman1->playername << endl;
                        (score_board->score) += 3;
                        (score_board->batsman1->playerscore) += 3;
                        score_board->swapbatsmen();
                        (score_board->baller->ballscore) += 3;
                        screenrefresh(score_board, commentar);


                    }
                    else if (batsmen_k<int((score_board->batsman1->battingsuccessrate) * 0.6)) {
                        //output double run
                        cout << " Brilliant double run by " << score_board->batsman1->playername << endl;
                        (score_board->score) += 2;
                        (score_board->batsman1->playerscore) += 2;
                        (score_board->baller->ballscore) += 2;
                        commentar->addcomment(5);
                        screenrefresh(score_board, commentar);


                    }
                    else {
                        //output  run
                        cout << " Brilliant run by " << score_board->batsman1->playername << endl;
                        (score_board->score)++;
                        (score_board->batsman1->playerscore)++;
                        score_board->swapbatsmen();
                        (score_board->baller->ballscore) += 1;
                        commentar->addcomment(4);
                        screenrefresh(score_board, commentar);
                    }

                }
                else {
                    cout << "ooh a wide ball\n";
                    (score_board->score)++;
                    (score_board->baller->ballscore) += 1;
                    screenrefresh(score_board, commentar);
                    i--;
                }
                // this_thread::sleep_for(three_sec);
                commentar->cur_comment = " ";
                // system("cls");

            }
            score_board->overs++;
            score_board->balls = 0;
            cout << "thats the end of this over. total overs are " << score_board->overs << " & total runs are " << score_board->score << endl;
            score_board->getnextballer();
            score_board->swapbatsmen();
            cout << "now balling " << score_board->baller->playername;
            screenrefresh(score_board, commentar);
            // this_thread::sleep_for(three_sec);
            // system("cls");
            commentar->cur_comment = " ";
        }
        if (k != 1) {
            cout << "\n\nThat is the end of the first half. now " << score_board->team2->teamname << " will bat & " << score_board->team1->teamname << " will ball." << endl << endl << endl;
            score_board->team1->totalruns = score_board->score;
            score_board->endoffirsthalf();
            screenrefresh(score_board, commentar);
            this_thread::sleep_for(three_sec);
            system("cls");
        }
    }
    return 0;
}

int main() {
    cricketmatch match1;

    fstream myfile;
    myfile.open("inputs.txt", ios::in);
    if (myfile.is_open()) {
        string line;
        while (getline(myfile, line)) {
            textprep(line, match1);
        }
    }
    else {
        cout << "File not found";
    }

    scoreboard* scoreboardnew = new scoreboard(match1.team1, match1.team2);
    comment* commentater_1 = new comment(scoreboardnew);
    // showscreen(scoreboardnew);
    // match1.team1->viewplayers();
    // match1.team2->viewplayers();
    // cout<<"\n***********\n";
    // scoreboardnew.showbatsman();
    // scoreboardnew.showballer();
    // cout<<"\nBatman 1 is out\n";
    // scoreboardnew.getnextbatsman(scoreboardnew.batsman1);
    // scoreboardnew.showbatsman();
    // scoreboardnew.showballer();
    // cout<<"\nBatman 2 is out\n";
    // scoreboardnew.getnextbatsman(scoreboardnew.batsman2);
    // scoreboardnew.showbatsman();
    // scoreboardnew.showballer();
    // cout<<"end of over\n";
    // scoreboardnew.swapbatsmen();
    // scoreboardnew.showbatsman();
    // scoreboardnew.getnextballer();
    // scoreboardnew.showballer();
    // cout<<"\n\nend of first half\n\n";
    // scoreboardnew.endoffirsthalf();
    // scoreboardnew.showballer();
    // scoreboardnew.showbatsman();
    // scoreboardnew.showballer();
    // // cout<<scoreboardnew.batsman1->playername;
    // cout<<"Batman 1 is out\n";
    // scoreboardnew.getnextbatsman(scoreboardnew.batsman1);
    // scoreboardnew.showbatsman();
    // scoreboardnew.showballer();
    // cout<<"Batman 2 is out\n";
    // scoreboardnew.getnextbatsman(scoreboardnew.batsman2);
    // scoreboardnew.showbatsman();
    // scoreboardnew.showballer();
    // cout<<"end of over\n";
    // scoreboardnew.swapbatsmen();
    // scoreboardnew.showbatsman();
    // scoreboardnew.showballer();
    OSsystem(scoreboardnew, commentater_1);


    return 0;


}
