#include "S5mod_right_hand.h"
#include <iostream>
// declaring 5 different constructors, we call third constructor
right_hand::right_hand(){
}

right_hand::right_hand(VectorXd q_ra,VectorXd r_target,MatrixXd R_target)
{
    HO_FK_right_palm(q_ra);
    dist=distance(r_target,r_right_palm);
    sai_target=sai_calc(R_target);
    theta_target=theta_calc(R_target);
    phi_target=phi_calc(R_target);
}

right_hand::right_hand(VectorXd q_ra,VectorXd v,VectorXd r_target,MatrixXd R_target)
{
    HO_FK_right_palm(q_ra);
    dist=distance(r_target,r_right_palm);
    sai_target=sai_calc(R_target);
    theta_target=theta_calc(R_target);
    phi_target=phi_calc(R_target);

    V.resize(3,1); 
    V=v;

    sai_dot=(sai_target-sai); 
    phi_dot=(phi_target-phi);
    theta_dot=(theta_target-theta);

    euler2w();
    jacob(q_ra);
}

right_hand::right_hand(VectorXd q_ra,VectorXd r_target,MatrixXd R_target,int i,double d0)
{
HO_FK_right_palm(q_ra);
dist=distance(r_target,r_right_palm);
sai_target=sai_calc(R_target);
theta_target=theta_calc(R_target);
phi_target=phi_calc(R_target);

V.resize(3,1);
double v_coef=v_des*min(float(i)/100.0,1.0)*pow(atan(dist/d0*20.0)/M_PI*2,2)/dist; // create smoth varying coef based on atan function (from 1 to 0)
V=v_coef*(r_target-r_right_palm);

double oriet_coef=1;//pow(tanh(3*(d_orient-dist)/d_orient),2); // create smoth varying coef based on tanh function (from 1 to 0)
sai_dot=oriet_coef*(sai_target-sai);
phi_dot=oriet_coef*(phi_target-phi);
theta_dot=oriet_coef*(theta_target-theta);

euler2w();
jacob(q_ra);
}

right_hand::right_hand(VectorXd q_ra,VectorXd r_target,MatrixXd R_target,double d0,double v_0, double v__target)
{
HO_FK_right_palm(q_ra);
dist=distance(r_target,r_right_palm);
sai_target=sai_calc(R_target);
theta_target=theta_calc(R_target);
phi_target=phi_calc(R_target);

V.resize(3,1);
//double v_coef=v_des*min(float(i)/100.0,1.0)*pow(atan(dist/d0*20.0)/M_PI*2,2)/dist;
v0=v_0;
v_target=v__target;
double v_coef=velocity(dist,d0);
V=v_coef*(r_target-r_right_palm)/dist;

double oriet_coef=1;//pow(tanh(3*(d_orient-dist)/d_orient),2);
sai_dot=oriet_coef*(sai_target-sai);
phi_dot=oriet_coef*(phi_target-phi);
theta_dot=oriet_coef*(theta_target-theta);

euler2w();
jacob(q_ra);
}



// declaring 4 different update_right_hand function, we call second one
void right_hand::update_right_hand(VectorXd q_ra,VectorXd r_target,MatrixXd R_target)
{  
HO_FK_right_palm(q_ra);
dist=distance(r_target,r_right_palm);
sai_target=sai_calc(R_target);
theta_target=theta_calc(R_target);
phi_target=phi_calc(R_target);
}

void right_hand::update_right_hand(VectorXd q_ra,VectorXd v,VectorXd r_target,MatrixXd R_target)
{
    HO_FK_right_palm(q_ra);
    dist=distance(r_target,r_right_palm);
    sai_target=sai_calc(R_target);
    theta_target=theta_calc(R_target);
    phi_target=phi_calc(R_target);

    V.resize(3,1);
    V=v;

    double oriet_coef=1;//pow(tanh(3*(d_orient-dist)/d_orient),2);
    sai_dot=oriet_coef*(sai_target-sai);
    phi_dot=oriet_coef*(phi_target-phi);
    theta_dot=oriet_coef*(theta_target-theta);

    euler2w();
    jacob(q_ra);
}

void right_hand::update_right_hand(VectorXd q_ra,VectorXd r_target,MatrixXd R_target,int i,double d0)
{
HO_FK_right_palm(q_ra);
dist=distance(r_target,r_right_palm);
sai_target=sai_calc(R_target);
theta_target=theta_calc(R_target);
phi_target=phi_calc(R_target);

V.resize(3,1);
double v_coef=v_des*min(float(i)/100.0,1.0)*pow(atan(dist/d0*20.0)/M_PI*2,2)/dist;
V=v_coef*(r_target-r_right_palm);

double oriet_coef=1;//pow(tanh(3*(d_orient-dist)/d_orient),2);
sai_dot=oriet_coef*(sai_target-sai);
phi_dot=oriet_coef*(phi_target-phi);
theta_dot=oriet_coef*(theta_target-theta);

euler2w();
jacob(q_ra);
}

void right_hand::update_right_hand(VectorXd q_ra,VectorXd r_target,MatrixXd R_target,double d0,double v_0, double v__target)
{
HO_FK_right_palm(q_ra);
dist=distance(r_target,r_right_palm);
sai_target=sai_calc(R_target);
theta_target=theta_calc(R_target);
phi_target=phi_calc(R_target);

V.resize(3,1);
//double v_coef=v_des*min(float(i)/100.0,1.0)*pow(atan(dist/d0*20.0)/M_PI*2,2)/dist;
v0=v_0;
v_target=v__target;
double v_coef=velocity(dist,d0);
V=v_coef*(r_target-r_right_palm)/dist;

double oriet_coef=1;//pow(tanh(3*(d_orient-dist)/d_orient),2);
sai_dot=oriet_coef*(sai_target-sai);
phi_dot=oriet_coef*(phi_target-phi);
theta_dot=oriet_coef*(theta_target-theta);

euler2w();
jacob(q_ra);
}



// void right_hand::matrix_view(MatrixXd M){
//     for (int i = 0; i <M.rows() ; ++i) {
//         QString str;
//         for (int j = 0; j <M.cols() ; ++j) {str+=QString::number(M(i,j));str+="   ";}
//         qDebug()<<str;}qDebug()<<"";}

// void right_hand::matrix_view(VectorXd M){
//     QString str;
//     for (int i = 0; i <M.rows() ; ++i) {str+=QString::number(M(i));str+="   ";}
//     qDebug()<<str;qDebug()<<"";}



double right_hand::toRad(double d){
    double r;
    r=d*M_PI/180;
    return r;
}

    
double  right_hand::phi_calc(MatrixXd R){
    if(R(1,0)==1 || R(1,0)==-1){return atan2(R(0,2),R(2,2));}
    else{return atan2(-R(2,0),R(0,0));}
}
double  right_hand::theta_calc(MatrixXd R){
    return asin(R(1,0));
}
double  right_hand::sai_calc(MatrixXd R){

    if(R(1,0)==1 || R(1,0)==-1){return 0;}
    else{return atan2(-R(1,2),R(1,1));}
}


MatrixXd right_hand::rot(int axis , double q ,int dim){
    if (dim==3){
        MatrixXd R(3,3);
    if (axis==1){
        R<<1,0,0,
                0,cos(q),-sin(q),
                0,sin(q),cos(q);
    }

    if (axis==2){
        R<<cos(q),0,sin(q),
                0,1,0 ,
                -sin(q),0,cos(q);
    }

    if (axis==3){
                R<<cos(q),-sin(q),0,
                sin(q),cos(q),0,
                0,0,1;
    }
    return R;
    }

    if(dim==4){
                    MatrixXd R(4,4);
        if (axis==1){
            R<<1,0,0,0,
                    0,cos(q),-sin(q),0,
                    0,sin(q),cos(q),0,
                    0,0,0,1;
        }

        if (axis==2){
            R<<cos(q),0,sin(q),0,
                    0,1,0,0,
                    -sin(q),0,cos(q),0,
                    0,0,0,1;
        }

        if (axis==3){
            R<<cos(q),-sin(q),0,0,
                    sin(q),cos(q),0,0,
                    0,0,1,0,
                    0,0,0,1;
        }
        return R;
    }
};

MatrixXd right_hand::trans(int axis, double d){
    MatrixXd H(4,4);
    H=MatrixXd::Identity(4,4);
    H(axis-1,3)=d;
    return H;
}

MatrixXd right_hand::trans(Vector3d d){
    MatrixXd H(4,4);
    H=MatrixXd::Identity(4,4);
    H.block(0,3,3,1)=d;
    return H; 
}

double right_hand::distance(VectorXd V1,VectorXd V2){
    double d;
    d=sqrt(pow(V1(0)-V2(0),2)+pow(V1(1)-V2(1),2)+pow(V1(2)-V2(2),2));
    return d;
}

double right_hand::velocity(double d,double d0){ // coef calculation for velocity
    double a2,a3;
    a3=2*(v_target-v0)/d0/d0/d0;
    a2=-3/2*a3*d0;
    return v_target+a2*d*d+a3*d*d*d;
}
MatrixXd right_hand::ObjToNeck(double h_pitch, double h_roll, double h_yaw) {
    T0.resize(4, 4);
    T0 << cos(M_PI / 9), 0, sin(M_PI / 9), 0,
        0, 1, 0, 0,
        -sin(M_PI / 9), 0, cos(M_PI / 9), 0,
        0, 0, 0, 1;

    T1.resize(4, 4);
    T1 << 1, 0, 0, camera[0],
          0, 1, 0, camera[1],
          0, 0, 1, camera[2],
          0, 0, 0, 1;

    T2.resize(4, 4);
    T2 << 1, 0, 0, 0,
          0, cos(h_roll), -sin(h_roll), 0,
          0, sin(h_roll), cos(h_roll), head_PtoR,
          0, 0, 0, 1;

    T3.resize(4, 4);
    T3 << cos(h_pitch), 0, sin(h_pitch), 0,
          0, 1, 0, 0,
         -sin(h_pitch), 0, cos(h_pitch), 0,
          0, 0, 0, 1;

    T4.resize(4, 4);
    T4 << cos(h_yaw), -sin(h_yaw), 0, 0,
          sin(h_yaw),  cos(h_yaw), 0, 0,
          0, 0, 1, head_YtoP,
          0, 0, 0, 1;

    T5.resize(4, 4);
    T5 << 1, 0, 0, Shoulder2Head[0],
          0, 1, 0, Shoulder2Head[1],
          0, 0, 1, Shoulder2Head[2],
          0, 0, 0, 1;

    T_EEtobase.resize(4, 4);
    T_EEtobase = T5 * T4 * T3 * T2 * T1 * T0;
    return T_EEtobase;
}

MatrixXd right_hand::returnAngles(MatrixXd T_EEtobase) {

    output.resize(3,1);

    if (T_EEtobase(2, 0) != 1 && T_EEtobase(2, 0) != -1)
    {
        theta_pitch = -asin(T_EEtobase(2, 0));
        sai_roll = atan2(T_EEtobase(2, 1) / cos(theta_pitch), T_EEtobase(2, 2) / cos(theta_pitch));
        phi_yaw = atan2(T_EEtobase(1, 0) / cos(theta_pitch), T_EEtobase(0, 0) / cos(theta_pitch));
    }
    else{
            phi_yaw = 0;
            if (T_EEtobase(2, 0) != -1)
            {
                theta_pitch = M_PI / 2;
                sai_roll = atan2(T_EEtobase(0, 1), T_EEtobase(0, 2));
            }
            else{
                theta_pitch = -M_PI / 2;
                sai_roll = atan2(-T_EEtobase(0, 1), -T_EEtobase(0, 2));
                }
    }
    output<< phi_yaw,sai_roll,theta_pitch;
    return output;
}
void right_hand::HO_FK_right_palm(VectorXd q_ra){
    R1_fix_shd.resize(4,4);
    R2_fix_shd.resize(4,4);
    R1_ra .resize(4,4);
    R2_ra.resize(4,4);
    R3_ra.resize(4,4);
    R1_fix_elbow.resize(4,4);
    R2_fix_elbow.resize(4,4);
    R4_ra.resize(4,4);
    R5_ra.resize(4,4);
    R6_ra.resize(4,4);
    R7_ra.resize(4,4);

    R1_fix_shd=rot(1,-angle_fix_shd,4);
    R2_fix_shd=rot(1,angle_fix_shd,4);
    R1_ra = rot(2,q_ra(0),4);
    R2_ra = rot(1,q_ra(1),4);
    R3_ra = rot(3,q_ra(2),4);
    R1_fix_elbow=rot(3,+angle_fix_elbow,4);
    R2_fix_elbow=rot(3,-angle_fix_elbow,4);
    R4_ra = rot(2,q_ra(3),4);
    R5_ra = rot(3,q_ra(4),4);
    R6_ra = rot(1,q_ra(5),4);
    R7_ra = rot(2,q_ra(6),4);

    P_arm_ra.resize(4,4);
    P_forearm_ra.resize(4,4);
    P_palm_ra.resize(4,4);

    P_arm_ra = trans(3,-L_arm);
    P_forearm_ra = trans(3,-L_forearm);
    P_palm_ra = trans(3,-L_palm);

    T_right_palm.resize(4,4);
    // FK formula
    T_right_palm=R1_fix_shd*R1_ra*R2_fix_shd*R2_ra*R3_ra*P_arm_ra*R1_fix_elbow*R4_ra*R5_ra*P_forearm_ra*R2_fix_elbow*R6_ra*R7_ra*P_palm_ra;

    r_right_palm.resize(3,1);
    r_right_palm<<T_right_palm.block(0,3,3,1);
    R_right_palm.resize(3,3);
    R_right_palm<<T_right_palm.block(0,0,3,3);

    theta=theta_calc(R_right_palm);
    sai=sai_calc(R_right_palm);
    phi=phi_calc(R_right_palm);
    //cout<<theta<<','<<sai<<','<<phi<<endl;
}



void right_hand::euler2w(){
//w_right_palm<<sin(sai)*sin(theta)*phi_dot+cos(sai)*theta_dot,
//        cos(sai)*sin(theta)*phi_dot-sin(sai)*theta_dot,
//        cos(theta)*phi_dot+sai_dot;

//w_right_palm<<cos(phi)*theta_dot + sin(phi)*sin(theta)*sai_dot,
//        sin(phi)*theta_dot - cos(phi)*sin(theta)*sai_dot,
//        cos(theta)*sai_dot + phi_dot;
//w_right_palm=-w_right_palm;
            // main
    w_right_palm<< sin(phi)*theta_dot + cos(phi)*cos(theta)*sai_dot,
            sin(theta)*sai_dot + phi_dot,
            cos(phi)*theta_dot - cos(theta)*sin(phi)*sai_dot;

        // cout<<w_right_palm(0)<<','<<w_right_palm(1)<<','<<w_right_palm(2)<<endl;

}
// wrist IK

VectorXd right_hand::wrist_right_calc(double alpha, double beta){

double phi;              
double gama ;           
double r1;           double pz;
double a1;           double b1;
double theta_32;     double theta_22;
double theta_12;      double theta_12_;
double k;
double a;             double b;
VectorXd result(2);


phi=248.5591*M_PI/180;
gama=111.4409*M_PI/180;
pz=-63.6749;
r1=28.0405;

a1=9.4067;   b1=60.9414;


VectorXd r1_(3);
r1_<<r1*cos(phi), r1*sin(phi),0;

VectorXd h1_P(3);
h1_P<<18.8173,18.6175,0.07835;

VectorXd P(3);
P<<0,0,pz;

MatrixXd R_PtoO;
R_PtoO.resize(3,3);
R_PtoO=rot(3,gama,3)*rot(1,alpha,3)*rot(2,beta,3);
//cout<<R_PtoO<<endl;

MatrixXd h1_O;
h1_O.resize(3,3);
h1_O=P+R_PtoO*h1_P;

MatrixXd R_OtoA1;
R_OtoA1.resize(3,3);
R_OtoA1=rot(3,phi,3);

MatrixXd h1_A1;
h1_A1.resize(3,1);
h1_A1=r1_+R_OtoA1*h1_O;
/*
VectorXd C(3);
C=h1_A1;
*/
theta_32=acos(h1_A1(1)/b1);
//cout<<theta_32<<endl;

k=(pow(h1_A1(0),2)+pow(h1_A1(1),2)+pow(h1_A1(2),2)-pow(a1,2)-pow(b1,2))/(2*a1*b1*sin(theta_32));


theta_22=acos(k);
//cout<<theta_22<<endl;


a=a1+b1*cos(theta_22)*sin(theta_32);
b=b1*sin(theta_22)*sin(theta_32);

//cout<<a<<endl<<b;

theta_12= -asin((a*h1_A1(2)+b*sqrt(pow(a,2)+pow(b,2)-pow(h1_A1(2),2)))/(pow(a,2)+pow(b,2)));
theta_12_= -asin((a*h1_A1(2)-b*sqrt(pow(a,2)+pow(b,2)-pow(h1_A1(2),2)))/(pow(a,2)+pow(b,2)));
//cout<<theta_12*180/M_PI<<endl<<theta_12_*180/M_PI;

result<< theta_12*180/M_PI,theta_12_*180/M_PI;
return result;
};

VectorXd right_hand::wrist_left_calc(double alpha, double beta){

double phi;              
double gama ;           
double r1;           double pz;
double a1;           double b1;
double theta_31;     double theta_21;
double theta_11;      double theta_11_;
double k;
double a;             double b;
VectorXd result(2);


phi=292.197718*M_PI/180;
gama=67.80228*M_PI/180;
pz=-86.07491;
r1=28.18923;

a1=9.36598;   b1=87.80285;


VectorXd r1_(3);
r1_<<r1*cos(phi), r1*sin(phi),0;

VectorXd h1_P(3);
h1_P<<-17.1750,19.356,0.0783;

VectorXd P(3);
P<<0,0,pz;

MatrixXd R_PtoO;
R_PtoO.resize(3,3);
R_PtoO=rot(3,gama,3)*rot(1,alpha,3)*rot(2,beta,3);
//cout<<R_PtoO<<endl;

MatrixXd h1_O;
h1_O.resize(3,3);
h1_O=P+R_PtoO*h1_P;

MatrixXd R_OtoA1;
R_OtoA1.resize(3,3);
R_OtoA1=rot(3,phi,3);

MatrixXd h1_A1;
h1_A1.resize(3,1);
h1_A1=r1_+R_OtoA1*h1_O;
/*
VectorXd C(3);
C=h1_A1;
*/
theta_31=acos(h1_A1(1)/b1);
//cout<<theta_32<<endl;

k=(pow(h1_A1(0),2)+pow(h1_A1(1),2)+pow(h1_A1(2),2)-pow(a1,2)-pow(b1,2))/(2*a1*b1*sin(theta_31));


theta_21=acos(k);
//cout<<theta_22<<endl;


a=a1+b1*cos(theta_21)*sin(theta_31);
b=b1*sin(theta_21)*sin(theta_31);

//cout<<a<<endl<<b;

theta_11= -asin((a*h1_A1(2)+b*sqrt(pow(a,2)+pow(b,2)-pow(h1_A1(2),2)))/(pow(a,2)+pow(b,2)));
theta_11_= -asin((a*h1_A1(2)-b*sqrt(pow(a,2)+pow(b,2)-pow(h1_A1(2),2)))/(pow(a,2)+pow(b,2)));
//cout<<theta_12*180/M_PI<<endl<<theta_12_*180/M_PI;

result<< theta_11*180/M_PI,theta_11_*180/M_PI;
return result;
};

// move from theta = 0 to theta = max with smooth minimum jerk function
double right_hand::move2pose(double max,double t_local,double T_start ,double T_end){
    double T_move=T_end-T_start;
    double c3=(10*max)/pow(T_move,3);
    double c4=-(15*max)/pow(T_move,4);
    double c5=(6*max)/pow(T_move,5);
    double theta=0;
    if(t_local<T_start){theta=0;}
    else if (t_local<T_end){theta=c3*pow(t_local-T_start,3)+c4*pow(t_local-T_start,4)+c5*pow(t_local-T_start,5);}
    else{theta=max;}
    return theta;
};
void  right_hand::SendGazebo(vector<double> q){
    ros::NodeHandle nh;
    
    ros::Publisher pub1  ;ros::Publisher pub2  ;ros::Publisher pub3  ;ros::Publisher pub4  ;
    ros::Publisher pub5  ;ros::Publisher pub6  ;ros::Publisher pub7  ;ros::Publisher pub8  ;
    ros::Publisher pub9  ;ros::Publisher pub10 ;ros::Publisher pub11 ;ros::Publisher pub12 ;
    ros::Publisher pub13 ;ros::Publisher pub14 ;ros::Publisher pub15 ;ros::Publisher pub16 ;
    ros::Publisher pub17 ;ros::Publisher pub18 ;ros::Publisher pub19 ;ros::Publisher pub20 ;
    ros::Publisher pub21 ;ros::Publisher pub22 ;ros::Publisher pub23 ;ros::Publisher pub24 ;
    ros::Publisher pub25 ;ros::Publisher pub26 ;ros::Publisher pub27 ;ros::Publisher pub28 ;
    ros::Publisher pub29 ;


    pub1  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint1_position_controller/command",100);
    pub2  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint2_position_controller/command",100);
    pub3  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint3_position_controller/command",100);
    pub4  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint4_position_controller/command",100);
    pub5  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint5_position_controller/command",100);
    pub6  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint6_position_controller/command",100);
    pub7  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint7_position_controller/command",100);
    pub8  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint8_position_controller/command",100);
    pub9  = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint9_position_controller/command",100);
    pub10 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint10_position_controller/command",100);
    pub11 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint11_position_controller/command",100);
    pub12 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint12_position_controller/command",100);
    pub13 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint13_position_controller/command",100);
    pub14 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint14_position_controller/command",100);
    pub15 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint15_position_controller/command",100);
    pub16 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint16_position_controller/command",100);
    pub17 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint17_position_controller/command",100);
    pub18 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint18_position_controller/command",100);
    pub19 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint19_position_controller/command",100);
    pub20 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint20_position_controller/command",100);
    pub21 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint21_position_controller/command",100);
    pub22 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint22_position_controller/command",100);
    pub23 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint23_position_controller/command",100);
    pub24 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint24_position_controller/command",100);
    pub25 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint25_position_controller/command",100);
    pub26 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint26_position_controller/command",100);
    pub27 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint27_position_controller/command",100);
    pub28 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint28_position_controller/command",100);
    pub29 = nh.advertise<std_msgs::Float64>("surenaVgazebo/joint29_position_controller/command",100);


    std_msgs::Float64 data;
    data.data=q[0];
    pub1.publish(data);
    data.data=q[1];
    pub2.publish(data);
    data.data=q[2];
    pub3.publish(data);
    data.data=q[3];
    pub4.publish(data);
    data.data=q[4];
    pub5.publish(data);
    data.data=q[5];
    pub6.publish(data);
    data.data=q[6];
    pub7.publish(data);
    data.data=q[7];
    pub8.publish(data);
    data.data=q[8];
    pub9.publish(data);
    data.data=q[9];
    pub10.publish(data);
    data.data=q[10];
    pub11.publish(data);
    data.data=q[11];
    pub12.publish(data);
    data.data=q[12];
    pub13.publish(data);
    data.data=q[13];
    pub14.publish(data);
    data.data=q[14];
    pub15.publish(data);
    data.data=q[15];
    pub16.publish(data);
    data.data=q[16];
    pub17.publish(data);
    data.data=q[17];
    pub18.publish(data);
    data.data=q[18];
    pub19.publish(data);
    data.data=q[19];
    pub20.publish(data);
    data.data=q[20];
    pub21.publish(data);
    data.data=q[21];
    pub22.publish(data);
    data.data=q[22];
    pub23.publish(data);
    data.data=q[23];
    pub24.publish(data);
    data.data=q[24];
    pub25.publish(data);
    data.data=q[25];
    pub26.publish(data);
    data.data=q[26];
    pub27.publish(data);
    data.data=q[27];
    pub28.publish(data);
    data.data=q[28];
    pub29.publish(data);
}

void right_hand::jacob(VectorXd q_ra){

    J_right_palm.resize(3,7);
    J_w_right_palm.resize(3,7);

    J_right_palm<<                                                                                                                                                                                                                                                                                                                                                                      L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) - cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - cos(q_ra(3))*(sin(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))) - sin(q_ra(4))*(sin(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) + cos(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))) + sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) + cos(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)))) + sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) - cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - cos(q_ra(3))*(sin(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))))) + cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) + cos(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)))) + sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) - cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - cos(q_ra(3))*(sin(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) - cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - cos(q_ra(3))*(sin(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))) - sin(q_ra(4))*(sin(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) + cos(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)))))) - cos(q_ra(5))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) - cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) + sin(q_ra(3))*(sin(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))))) - L_arm*(cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) - cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - L_forearm*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) - cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) + sin(q_ra(3))*(sin(angle_fix_elbow)*(sin(q_ra(0))*sin(q_ra(2)) + cos(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow)*(cos(q_ra(2))*sin(q_ra(0)) - sin(q_ra(2))*(cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   L_palm*(cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) + cos(angle_fix_elbow + q_ra(2))*cos(q_ra(4))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - cos(angle_fix_elbow + q_ra(2))*sin(q_ra(4))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2))) - cos(q_ra(5))*(cos(q_ra(3))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2) + sin(angle_fix_elbow + q_ra(2))*sin(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2))) + sin(q_ra(6))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - cos(angle_fix_elbow + q_ra(2))*sin(q_ra(4))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) + cos(angle_fix_elbow + q_ra(2))*cos(q_ra(4))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)))) - L_forearm*(cos(q_ra(3))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2) + sin(angle_fix_elbow + q_ra(2))*sin(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - L_arm*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(q_ra(3))*cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - cos(q_ra(3))*sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) - cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - cos(q_ra(3))*sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(q_ra(3))*cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) - cos(q_ra(5))*sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) + L_forearm*sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           - L_forearm*(cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0) - q_ra(2))/2 + cos(q_ra(0) + q_ra(2))/2 - sin(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - sin(angle_fix_elbow)*(sin(q_ra(0) + q_ra(2))/2 - sin(q_ra(0) - q_ra(2))/2 + cos(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2))) - sin(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - L_palm*(cos(q_ra(6))*(cos(q_ra(5))*(cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0) - q_ra(2))/2 + cos(q_ra(0) + q_ra(2))/2 - sin(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - sin(angle_fix_elbow)*(sin(q_ra(0) + q_ra(2))/2 - sin(q_ra(0) - q_ra(2))/2 + cos(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2))) - sin(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2)) + sin(angle_fix_elbow - q_ra(4))*sin(q_ra(5))*(sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0) - q_ra(2))/2 + cos(q_ra(0) + q_ra(2))/2 - sin(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - sin(angle_fix_elbow)*(sin(q_ra(0) + q_ra(2))/2 - sin(q_ra(0) - q_ra(2))/2 + cos(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2))) + cos(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2))) - sin(q_ra(6))*cos(angle_fix_elbow - q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0) - q_ra(2))/2 + cos(q_ra(0) + q_ra(2))/2 - sin(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2)) - sin(angle_fix_elbow)*(sin(q_ra(0) + q_ra(2))/2 - sin(q_ra(0) - q_ra(2))/2 + cos(q_ra(2))*(cos(angle_fix_shd + q_ra(0) + q_ra(1))/2 - cos(angle_fix_shd - q_ra(0) + q_ra(1))/2))) + cos(q_ra(3))*(sin(angle_fix_shd + q_ra(0) + q_ra(1))/2 - sin(angle_fix_shd - q_ra(0) + q_ra(1))/2))),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))))))) + cos(q_ra(6))*sin(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))))),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       -L_palm*cos(q_ra(6))*(cos(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))))))) - sin(q_ra(5))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                L_palm*(sin(q_ra(6))*(cos(q_ra(5))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))))) + sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))))) + cos(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))))),
     L_forearm*(sin(q_ra(3))*(cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + cos(angle_fix_shd + q_ra(1))*cos(q_ra(3))*sin(angle_fix_shd)*sin(q_ra(0))) - L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3)))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3))))) - cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3)))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3))))) + cos(q_ra(5))*(sin(q_ra(3))*(cos(angle_fix_elbow)*sin(angle_fix_shd)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + sin(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + cos(angle_fix_shd + q_ra(1))*cos(q_ra(3))*sin(angle_fix_shd)*sin(q_ra(0))))) + L_arm*cos(angle_fix_shd + q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)), L_palm*(cos(q_ra(6))*(cos(q_ra(5))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) + sin(angle_fix_elbow + q_ra(2))*sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - sin(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) + cos(angle_fix_elbow + q_ra(2))*cos(q_ra(4))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow + q_ra(2))*sin(q_ra(4))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))) - sin(q_ra(6))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) - cos(angle_fix_elbow + q_ra(2))*sin(q_ra(4))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) - sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))) + cos(angle_fix_elbow + q_ra(2))*cos(q_ra(4))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))))) + L_arm*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) + L_forearm*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(1)) + cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1))) + sin(angle_fix_elbow + q_ra(2))*sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_shd)*sin(q_ra(1)) + cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd))), - L_palm*(cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) + cos(q_ra(3))*sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) - cos(q_ra(3))*cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))))) + cos(q_ra(5))*sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) - cos(q_ra(3))*cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) + cos(q_ra(3))*sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))))) - L_forearm*sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))), - L_forearm*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*cos(q_ra(4))*(cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(angle_fix_elbow)*sin(q_ra(4))*(cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))))) + cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*sin(q_ra(4))*(cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(q_ra(4))*sin(angle_fix_elbow)*(cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))))) + cos(q_ra(5))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))))),  L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))))) - cos(q_ra(6))*sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))))), -L_palm*cos(q_ra(6))*(sin(q_ra(5))*(cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))))), -L_palm*(sin(q_ra(6))*(cos(q_ra(5))*(cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))))) + cos(q_ra(6))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))))),
     L_forearm*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + cos(angle_fix_shd + q_ra(1))*cos(angle_fix_shd)*cos(q_ra(3))*sin(q_ra(0))) + L_palm*(cos(q_ra(6))*(cos(q_ra(5))*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + cos(angle_fix_shd + q_ra(1))*cos(angle_fix_shd)*cos(q_ra(3))*sin(q_ra(0))) + sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3)))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3)))))) - sin(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3)))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(q_ra(4))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))*sin(q_ra(2)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) + cos(angle_fix_shd)*sin(angle_fix_elbow)*(cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))*sin(q_ra(1)) - cos(q_ra(0))*sin(q_ra(2)) + cos(q_ra(1))*cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))) - cos(angle_fix_shd + q_ra(1))*cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(3)))))) + L_arm*cos(angle_fix_shd + q_ra(1))*cos(angle_fix_shd)*sin(q_ra(0)), L_palm*(cos(q_ra(6))*(cos(q_ra(5))*(cos(q_ra(3))*(sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)) - sin(angle_fix_elbow + q_ra(2))*sin(q_ra(3))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))) - sin(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)) + sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))) - cos(angle_fix_elbow + q_ra(2))*cos(q_ra(4))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)) + sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))) + cos(angle_fix_elbow + q_ra(2))*sin(q_ra(4))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))))) - sin(q_ra(6))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)) + sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))) + cos(angle_fix_elbow + q_ra(2))*sin(q_ra(4))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)) + sin(angle_fix_elbow + q_ra(2))*cos(q_ra(3))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))) - cos(angle_fix_elbow + q_ra(2))*cos(q_ra(4))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))))) + L_arm*(sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)) + L_forearm*(cos(q_ra(3))*(sin(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1))*sin(angle_fix_shd)) - sin(angle_fix_elbow + q_ra(2))*sin(q_ra(3))*(cos(q_ra(1))*sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))*cos(q_ra(1)) + cos(angle_fix_shd)*sin(angle_fix_shd)*sin(q_ra(1)) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd)*sin(q_ra(1)))),   L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) - cos(q_ra(3))*cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) + cos(q_ra(3))*sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) - cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) + cos(q_ra(3))*sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) - cos(q_ra(3))*cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) + cos(q_ra(5))*sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) - L_forearm*sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))),   L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*cos(q_ra(4))*(cos(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(angle_fix_elbow)*sin(q_ra(4))*(cos(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))))) + cos(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*sin(q_ra(4))*(cos(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(q_ra(4))*sin(angle_fix_elbow)*(cos(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))))) + cos(q_ra(5))*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))))) + L_forearm*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))), -L_palm*(sin(q_ra(6))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) - cos(q_ra(6))*sin(q_ra(5))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))))),  L_palm*cos(q_ra(6))*(cos(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) + sin(q_ra(5))*(cos(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))))),  L_palm*(cos(q_ra(6))*(cos(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) + sin(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) - sin(q_ra(6))*(sin(q_ra(5))*(cos(angle_fix_elbow)*(sin(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) - sin(angle_fix_elbow)*(cos(q_ra(4))*(sin(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) - cos(q_ra(5))*(cos(q_ra(3))*(cos(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(sin(angle_fix_shd)*sin(angle_fix_shd) + cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0))) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))))));




      J_w_right_palm<<                 0,                     cos(q_ra(0)),                                                                                                   cos(angle_fix_shd + q_ra(1))*sin(q_ra(0)),                                                                                                                                                                                                                     - cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))),                                                                                                                                                                                                                                                                                                               cos(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       - cos(q_ra(4))*(sin(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(angle_fix_elbow)*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) - sin(q_ra(4))*(cos(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(angle_fix_elbow)*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))),                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         sin(q_ra(5))*(cos(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))))) - cos(q_ra(5))*(cos(q_ra(4))*(cos(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(angle_fix_elbow)*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))))) - sin(q_ra(4))*(sin(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) + sin(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0))))) + cos(angle_fix_elbow)*(sin(q_ra(3))*(cos(angle_fix_shd)*cos(q_ra(1))*sin(q_ra(0)) - sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(cos(q_ra(0))*cos(q_ra(2)) + sin(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))) - sin(angle_fix_elbow)*(cos(q_ra(0))*sin(q_ra(2)) - cos(q_ra(2))*(cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(1)) + cos(q_ra(1))*sin(angle_fix_shd)*sin(q_ra(0)))))))),
      cos(angle_fix_shd), -sin(angle_fix_shd)*sin(q_ra(0)), cos(angle_fix_shd)*cos(q_ra(1))*sin(angle_fix_shd)*(cos(q_ra(0)) - 1) - sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)), cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))), sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))) - cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))), cos(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) - sin(q_ra(4))*(sin(angle_fix_elbow)*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))), - cos(q_ra(5))*(cos(q_ra(4))*(sin(angle_fix_elbow)*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - cos(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))))) + sin(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + sin(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0)))))) - sin(q_ra(5))*(cos(q_ra(3))*(sin(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) + cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(2))*sin(angle_fix_shd)*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)+ cos(q_ra(0))*sin(angle_fix_shd)*sin(angle_fix_shd)) - sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))),
     -sin(angle_fix_shd), -cos(angle_fix_shd)*sin(q_ra(0)),          cos(q_ra(1))*(cos(angle_fix_shd)*cos(angle_fix_shd)*cos(q_ra(0)) - cos(angle_fix_shd)*cos(angle_fix_shd)+ 1) - (sin(2*angle_fix_shd)*sin(q_ra(1))*(cos(q_ra(0)) - 1))/2, cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))), cos(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))), sin(q_ra(4))*(sin(angle_fix_elbow)*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) - cos(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))),   cos(q_ra(5))*(cos(q_ra(4))*(sin(angle_fix_elbow)*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) + cos(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))))) + sin(q_ra(4))*(cos(angle_fix_elbow)*(sin(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))))) - sin(angle_fix_elbow)*(cos(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2))) - sin(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0)))))) + sin(q_ra(5))*(cos(q_ra(3))*(cos(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) + sin(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + sin(q_ra(3))*(cos(angle_fix_elbow)*(sin(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) - cos(angle_fix_shd)*cos(q_ra(2))*sin(q_ra(0))) + sin(angle_fix_elbow)*(cos(q_ra(2))*(sin(q_ra(1))*(cos(q_ra(0))*cos(angle_fix_shd)*cos(angle_fix_shd)+ sin(angle_fix_shd)*sin(angle_fix_shd)) - cos(q_ra(1))*(cos(angle_fix_shd)*sin(angle_fix_shd) - cos(angle_fix_shd)*cos(q_ra(0))*sin(angle_fix_shd))) + cos(angle_fix_shd)*sin(q_ra(0))*sin(q_ra(2)))));


};



void right_hand::doQP(VectorXd q_ra){
jacob(q_ra);

G.resize(7,7);
g.resize(7,1);

G=power*MatrixXd::Identity(7,7);
g.fill(0.0);

G=G+2*Right_palm_position_power*J_right_palm.transpose()*J_right_palm;
g=g-2* Right_palm_position_power*J_right_palm.transpose()*V;
if (dist<d_orient){
G=G+pow(tanh(5*(d_orient-dist)/d_orient),2)*Right_palm_orientation_power*2*J_w_right_palm.transpose()*J_w_right_palm;
g=g+pow(tanh(5*(d_orient-dist)/d_orient),2)*Right_palm_orientation_power*(-2)*J_w_right_palm.transpose()*w_right_palm;
}


vector<double> minimum(7);
vector<double> maximum(7);

minimum={-110.0,-90.0,-60.0,-90.0,-90.0,-20.0,-20.0};
maximum={80.0 ,-5.0 ,60.0 ,-1.0 ,90.0 ,20.0 ,20.0 };

    CI.resize(7,14);
    CI<<MatrixXd::Identity(7,7)*(-1),MatrixXd::Identity(7,7); 
    ci0.resize(14,1);
    ci0<<   min((toRad(maximum[0])-q_ra(0))/T,qdot_max),
            min((toRad(maximum[1])-q_ra(1))/T,qdot_max),
            min((toRad(maximum[2])-q_ra(2))/T,qdot_max),
            min((toRad(maximum[3])-q_ra(3))/T,qdot_max),
            min((toRad(maximum[4])-q_ra(4))/T,qdot_max),
            min((toRad(maximum[5])-q_ra(5))/T,qdot_max),
            min((toRad(maximum[6])-q_ra(6))/T,qdot_max),
            -max((toRad(minimum[0])-q_ra(0))/T,-qdot_max),
            -max((toRad(minimum[1])-q_ra(1))/T,-qdot_max),
            -max((toRad(minimum[2])-q_ra(2))/T,-qdot_max),
            -max((toRad(minimum[3])-q_ra(3))/T,-qdot_max),
            -max((toRad(minimum[4])-q_ra(4))/T,-qdot_max),
            -max((toRad(minimum[5])-q_ra(5))/T,-qdot_max),
            -max((toRad(minimum[6])-q_ra(6))/T,-qdot_max); 

CE.resize(0,0);

qdot.resize(7,1);
double optCosts;
optCosts = solve_quadprog(G, g, CE, ce0, CI, ci0, qdot);
VectorXd temp=CI.transpose()*qdot + ci0;

q_next.resize(7,1);
q_next=q_ra+T*qdot;
// cout<<qdot(0)<<','<<qdot(1)<<','<<qdot(2)<<','<<qdot(3)<<','<<qdot(4)<<','<<qdot(5)<<','<<qdot(6)<<endl;
// cout<<min((toRad(maximum[1])-q_ra(1))/T,qdot_max)<<","<<-max((toRad(minimum[1])-q_ra(1))/T,-qdot_max)<<endl;
if(q_next(0)<toRad(minimum[0])-.005){ q_next(0)=toRad(minimum[0]); cout<<"qr"<<1<<" out of range!";}
if(q_next(0)>toRad(maximum[0])+.005){ q_next(0)=toRad(maximum[0]); cout<<"qr"<<1<<" out of range!";}
if(q_next(1)<toRad(minimum[1])-.005){ q_next(1)=toRad(minimum[1]); cout<<"qr"<<2<<" out of range!";}
if(q_next(1)>toRad(maximum[1])+.005){ q_next(1)=toRad(maximum[1]); cout<<"qr"<<2<<" out of range!";}
if(q_next(2)<toRad(minimum[2])-.005){ q_next(2)=toRad(minimum[2]); cout<<"qr"<<3<<" out of range!";}
if(q_next(2)>toRad(maximum[2])+.005){ q_next(2)=toRad(maximum[2]); cout<<"qr"<<3<<" out of range!";}
if(q_next(3)<toRad(minimum[3])-.005){ q_next(3)=toRad(minimum[3]); cout<<"qr"<<4<<" out of range!";}
if(q_next(3)>toRad(maximum[3])+.005){ q_next(3)=toRad(maximum[3]); cout<<"qr"<<4<<" out of range!";}
if(q_next(4)<toRad(minimum[4])-.005){ q_next(4)=toRad(minimum[4]); cout<<"qr"<<5<<" out of range!";}
if(q_next(4)>toRad(maximum[4])+.005){ q_next(4)=toRad(maximum[4]); cout<<"qr"<<5<<" out of range!";}
if(q_next(5)<toRad(minimum[5])-.005){ q_next(5)=toRad(minimum[5]); cout<<"qr"<<6<<" out of range!";}
if(q_next(5)>toRad(maximum[5])+.005){ q_next(5)=toRad(maximum[5]); cout<<"qr"<<6<<" out of range!";}
if(q_next(6)<toRad(minimum[6])-.005){ q_next(6)=toRad(minimum[6]); cout<<"qr"<<7<<" out of range!";}
if(q_next(6)>toRad(maximum[6])+.005){ q_next(6)=toRad(maximum[6]); cout<<"qr"<<7<<" out of range!";}

}

double right_hand::wrist_pos2mot(double pos){
    double mot;
    mot=60*M_PI/63*tan(pos);
    return mot;
}

vector<int> right_hand::data2qc(vector<double> cntrl) {
    vector<int> qref(7);
    qref[0]=int((cntrl[0])*(1/(2*M_PI))*(2304)*120);
    qref[1]=int((cntrl[1])*(1/(2*M_PI))*(2304)*120);
    qref[2]=int((cntrl[2])*(1/(2*M_PI))*(2304)*120);
    qref[3]=int((cntrl[3])*(1/(2*M_PI))*(2304)*100);
    qref[4]=int(wrist_pos2mot(cntrl[4]));
    qref[5]=int(wrist_pos2mot(cntrl[5]));
    qref[6]=int(wrist_pos2mot(cntrl[6]));


    vector<double> minimum(7);
    vector<double> maximum(7);

    vector<int> minimumQC(7);
    vector<int> maximumQC(7);
    minimum={-90.0,-10.0,-45.0,-90.0,-60.0,-30.0,-30.0};
    maximum={45.0,90.0,45.0,-5.0,60.0,30.0,30.0};

    minimumQC[0]=int(minimum[0]*(1/(2*M_PI))*(2304)*120);
    minimumQC[1]=int(minimum[1]*(1/(2*M_PI))*(2304)*120);
    minimumQC[2]=int(minimum[2]*(1/(2*M_PI))*(2304)*120);
    minimumQC[3]=int(minimum[3]*(1/(2*M_PI))*(2304)*100);
    minimumQC[4]=int(wrist_pos2mot(minimum[4]));
    minimumQC[5]=int(wrist_pos2mot(minimum[5]));
    minimumQC[6]=int(wrist_pos2mot(minimum[6]));

    maximumQC[0]=int(maximum[0]*(1/(2*M_PI))*(2304)*120);
    maximumQC[1]=int(maximum[1]*(1/(2*M_PI))*(2304)*120);
    maximumQC[2]=int(maximum[2]*(1/(2*M_PI))*(2304)*120);
    maximumQC[3]=int(maximum[3]*(1/(2*M_PI))*(2304)*100);
    maximumQC[4]=int(wrist_pos2mot(maximum[4]));
    maximumQC[5]=int(wrist_pos2mot(maximum[5]));
    maximumQC[6]=int(wrist_pos2mot(maximum[6]));

    for (int i = 0; i < 12; ++i) {
        if(qref[i]<minimumQC[i]){qref[i]=minimumQC[i];}
        if(qref[i]>maximumQC[i]){qref[i]=maximumQC[i];}
    }


    for (int i = 0; i < 12; ++i) {
        if(qref[i]<minimumQC[i]){qref[i]=minimumQC[i];}
        if(qref[i]>maximumQC[i]){qref[i]=maximumQC[i];}
    }

    return qref;
}

vector<int> right_hand::data2qc_without_wrist(vector<double> cntrl) {
    vector<int> qref(4);
    qref[0]=int((cntrl[0])*(1/(2*M_PI))*(2304)*120);
    qref[1]=int((cntrl[1])*(1/(2*M_PI))*(2304)*120);
    qref[2]=int((cntrl[2])*(1/(2*M_PI))*(2304)*120);
    qref[3]=int((cntrl[3])*(1/(2*M_PI))*(2304)*100);


    vector<double> minimum(7);
    vector<double> maximum(7);

    vector<int> minimumQC(7);
    vector<int> maximumQC(7);
    minimum={-90.0,0.0,-45.0,-90.0,-60.0,-30.0,-30.0};
    maximum={45.0,90.0,45.0,-5.0,60.0,30.0,30.0};

    minimumQC[0]=int(minimum[0]*(1/(2*M_PI))*(2304)*120);
    minimumQC[1]=int(minimum[1]*(1/(2*M_PI))*(2304)*120);
    minimumQC[2]=int(minimum[2]*(1/(2*M_PI))*(2304)*120);
    minimumQC[3]=int(minimum[3]*(1/(2*M_PI))*(2304)*100);


    maximumQC[0]=int(maximum[0]*(1/(2*M_PI))*(2304)*120);
    maximumQC[1]=int(maximum[1]*(1/(2*M_PI))*(2304)*120);
    maximumQC[2]=int(maximum[2]*(1/(2*M_PI))*(2304)*120);
    maximumQC[3]=int(maximum[3]*(1/(2*M_PI))*(2304)*100);


    for (int i = 0; i < 12; ++i) {
        if(qref[i]<minimumQC[i]){qref[i]=minimumQC[i];}
        if(qref[i]>maximumQC[i]){qref[i]=maximumQC[i];}
    }


    for (int i = 0; i < 12; ++i) {
        if(qref[i]<minimumQC[i]){qref[i]=minimumQC[i];}
        if(qref[i]>maximumQC[i]){qref[i]=maximumQC[i];}
    }
    return qref;
}

