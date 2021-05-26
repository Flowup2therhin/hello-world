#include "C:\Gompper\Code\ABP\ActiveBrownianParticle.cpp"
//#include "/mnt/c/Gompper/Code/ABP/ActiveBrownianParticle.cpp"




int main(){
    // time business
    float T = 1000;      // complete time [s]
    float dt = 0.1;      // delta time [s]
    int tts = T/dt;      // total time steps (no dimension)


    // Create np ABP's and an environment and set up initial conditions:
    int np = 10;    //Number Particles (if i put this on 100, the programm is not running, why ever. Storage limits?)
    ActiveBrownianParticle ABP[np];
    environment Env;
    
    /*
    //experiment for generating ABP's with 'new' to use dynamic memory.
    ActiveBrownianParticle * test;
    test = new ActiveBrownianParticle[5];
    delete[] test;
    */

    srand(time(NULL)); // for randomnes, this really is better in python...
    for (int i = 0; i < np; i++){
        ABP[i].set_px((rand()%20-10)*pow(10,-6));
        ABP[i].set_py((rand()%20-10)*pow(10,-6));
        ABP[i].set_dt(dt);
        ABP[i].set_v(1);
        ABP[i].set_phi(2*M_PI*(double)rand()/(double)RAND_MAX);
        ABP[i].set_w(1);
    }


    cout << "bishiergekommen4" << endl;
    // Create array for storage
    float PositionArray[np][tts][2] = {0};
    float AngleArray[np][tts] = {0};
    float TimeArray[np][tts] = {0};

    cout << "bishiergekommen3" << endl;
    // perform simulation of ABP's for a series of time steps and store the position and angle information
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning = myclock::now();
    for (int i = 0; i < np; i++){
        PositionArray[i][0][0] = ABP[i].get_px();
        PositionArray[i][0][1] = ABP[i].get_py();
        AngleArray[i][0] = ABP[i].get_phi();
        TimeArray[i][0] = ABP[i].get_t();
    }  
    for (int j = 1; j < tts; j++){
        for (int i = 0; i < np; i++){
            ABP[i].update_2D_pBP_2(Env);
            PositionArray[i][j][0] = ABP[i].get_px();
            PositionArray[i][j][1] = ABP[i].get_py();
            AngleArray[i][j] = ABP[i].get_phi();
            TimeArray[i][j] = j*dt;
        }
    }
    
    cout << "bishiergekommen2" << endl;
    // perform statistical analysis
    float MSD[np][tts] = {0};
    for (int i = 0; i < tts; i++){
        for (int j = 0; j < np; j++){
            MSD[j][i] = sqrt(pow(PositionArray[j][i][0] - PositionArray[j][0][0], 2) + pow(PositionArray[j][i][1] - PositionArray[j][0][1], 2));
        }
    }
    float MSD_Mean[tts] = {0};
    for (int i = 0; i < tts; i++){
        for (int j = 0; j < np; j++){
            MSD_Mean[i] = MSD_Mean[i] + MSD[j][i];
        }
        MSD_Mean[i] = MSD_Mean[i]/np;
    }


    // Output
    cout << "bishiergekommen1" << endl;
    ofstream outputfile;

    std::string filename = "Output.csv";

    outputfile.open(filename);
    for (int i = 0; i < np; i++){
        if (i==0){
            outputfile << "px" << i << ",py" << i << ",MSD" << i;
        }
        else{
            outputfile << ",px" << i << ",py" << i << ",MSD" << i;
        }
    }
    outputfile << ",MSD_Mean" << ",time" << endl;
    for (int i = 0; i < tts; i++){
        for (int j = 0; j < np; j++){
            if (j==0){
                outputfile << PositionArray[j][i][0] << "," << PositionArray[j][i][1] << "," << MSD[j][i];
            }
            else{
                outputfile << "," << PositionArray[j][i][0] << "," << PositionArray[j][i][1] << "," << MSD[j][i];
            }
        }
        outputfile << "," << MSD_Mean[i] << "," << TimeArray[0][i] << endl;
    }
    outputfile.close();


    /*
    for (int i = 0; i < tts; i++){
        cout << "The x-Position:" << PositionArray[0][i][0] << "The y-Position:" << PositionArray[0][i][1] << "The angle phi:" << AngleArray[0][i] << endl;
    }
    for (int i = 0; i < tts; i++){
        cout << "The MSD:" << MSD[0][i] << endl;
    }
    for (int i = 0; i < tts; i++){
        cout << "Mean MSD:" << MSD_Mean[i] << endl;
    }
    */

    return 0;

    
}