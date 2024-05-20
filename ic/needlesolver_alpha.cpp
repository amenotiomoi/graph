#include "bits/stdc++.h"
#include "windows.h"
using namespace std;
#pragma GCC optimize("Ofast","inline","-ffast-math")
#pragma GCC target("avx,sse2,sse3,sse4,mmx")
struct object {
   int type;
   float x,y;
   static void typelist() {
      cout << "物体列表" << endl;
      cout << "type=1 死砖" << endl;
      cout << "type=2 刺(向上)" << endl;
      cout << "type=3 刺(向下)" << endl;
      cout << "type=4 刺(向左)" << endl;
      cout << "type=5 刺(向右)" << endl;
      cout << "type=6 起点,假设其正一格下方拥有一个无圆角平台" << endl;
      cout << "type=7 终点,假设其正一格下方拥有一个无圆角平台" << endl;
   }
};
struct map {
   static float uintToFloat(unsigned int uintValue) {
      float floatValue;
      memcpy(&floatValue,&uintValue,sizeof(float));
      return floatValue;
   }
   static unsigned int floatToUInt(float floatValue) {
      unsigned int intValue;
      memcpy(&intValue,&floatValue,sizeof(float));
      return intValue;
   }
   vector<object> obj;
   int x,y;
   void scanf(const string& input) {
      ifstream file(input);
      if(!file.is_open()) {
         cout << "在尝试打开\'" << input << "\'时出错,检查文件是否存在或者访问权限不足." << endl;
         return;
      }
      obj.clear();
      int n;file >> n;
      for(int i=0;i<n;++i) {
         object objItem;
         file >> objItem.type;
         unsigned int x_uint,y_uint;
         file >> x_uint >> y_uint;
         objItem.x=uintToFloat(x_uint);
         objItem.y=uintToFloat(y_uint);
         obj.push_back(objItem);
      }
      file >> x >> y;
      file.close();
   }
   void print(string output) {
      ofstream file(output);
      streambuf *backup=cout.rdbuf();
      cout.rdbuf(file.rdbuf());
      cout << obj.size() << endl;
      for(object i:obj)
         cout << i.type << ' ' << floatToUInt(i.x) << ' ' << floatToUInt(i.y) << endl;
      cout << x << ' ' << y << endl;
      cout.rdbuf(backup);
      file.close();
   }
} current;
struct frac {
   int a,b; // a/b
   frac sp() {
      if(a==0) return {0,1};
      int v=__gcd(abs(a),abs(b));
      return {(a*b<0?-1:1)*abs(a)/v,abs(b)/v};
   }
   frac operator+(const frac& v) const {return frac{a*v.b+b*v.a,b*v.b}.sp();}
   frac operator-(const frac& v) const {return frac{a*v.b-b*v.a,b*v.b}.sp();}
   frac operator*(const frac& v) const {return frac{a*v.a,b*v.b}.sp();}
   frac operator/(const frac& v) const {return frac{a*v.b,b*v.a}.sp();}
   bool operator==(const frac&v) const {return a*v.b==b*v.a;}
   bool operator<(const frac& v) const {
      if((a^v.a)<0) return a<v.a;
      if(a<0) return -a*v.b>b*-v.a;
      return a*v.b<b*v.a;
   }
};
struct vsqrt {
   int isqrt(int x){if(x==0)return 0;int a=1ll<<(__lg(x+1)/2+2),b=1ll<<(__lg(x+1)/2+1);while(a>b){a=b;b=(b+x/b)/2;}return a;}
   frac a,b; // sqrt(a)+b
   vsqrt sp() {
      a=a.sp();b=b.sp();
      if(a.a<0) return {a,b};
      int v1=isqrt(a.a),v2=isqrt(a.b);
      if(v1*v1==a.a&&v2*v2==a.b)
         return {{0,1},(b+frac{v1,v2})};
      return {a,b};
   }
   bool operator<(const vsqrt&v) const {
      if(b==v.b) return a<v.a;
      if(v.b<b&&v.a<(b-v.b)*(b-v.b)) {return false;}
      frac t=v.b-b;t=(a-v.a-t*t)/(t+t);
      if(t<frac{0,1}) return (b<v.b);
      return (t*t<v.a)^(v.b<b);
   }
   bool operator==(const vsqrt&v) const {
      return a==v.a&&b==v.b;
   }
};
struct chtholly_tree {
   set<pair<vsqrt,vsqrt>> tree;
   set<pair<vsqrt,vsqrt>>::iterator check(vsqrt x) {
      x=x.sp();
      auto itr=tree.upper_bound({x,x});
      if(itr!=tree.end()&&(*itr).first==x) return itr;
      if(itr==tree.begin()) return tree.end();
      return (*prev(itr)).second<x?tree.end():prev(itr);
   }
   bool insert(vsqrt l,vsqrt r) {
      auto il=check(l);
      auto ir=check(r);
      if(il==tree.end()) {
         if(ir==tree.end()) {
            auto itr=tree.lower_bound({l,l});
            while(itr!=tree.end()&&(*itr).second<r) itr=tree.erase(itr);
            tree.insert({l,r});
            return true;
         }
         auto itr=tree.lower_bound({l,l});
         while(!(r<(*itr).second)) itr=tree.erase(itr);
         vsqrt val=itr->second;
         tree.erase(itr);
         tree.insert({l,val});
         return true;
      }
      if(ir==tree.end()) {
         vsqrt val=il->first;
         while(il!=tree.end()&&(*il).second<r) il=tree.erase(il);
         tree.insert({val,r});
         return true;
      }
      if(il==ir) return false;
      vsqrt val1=il->first,val2=ir->second;
      while(il!=tree.end()&&!(r<(*il).second)) il=tree.erase(il);
      tree.insert({val1,val2});
      return true;
   }
};
struct pkid {
   double y,speed;
   double leftx,rightx;
   double speed;
   bool shiftdown,jump;
   static double sqr(double t) {return t*t;}
   vector<pkid> fix() {
      if(y<-0.04125||y>0.64*current.y-0.4225) return {};
      leftx=fmax(-0.2425,leftx);
      rightx=fmin(0.64*current.x-0.39625,rightx); // 边界判定
      vector<pkid> val={{y,leftx,rightx,speed,shiftdown,jump}};
      for(object a:current.obj) {
         vector<pkid> nq;
         for(pkid i:val) {
            if(a.type==6||a.type==7) {nq=val;continue;}
            if(a.type==1) {
               double val=0;
               if(a.y+0.565<=i.y&&i.y<a.y+0.615) {
                  val=sqrt(sqr(0.05)-sqr(i.y-0.565-a.y))+0.36375; // 上半部分的圆角
                  if(sqr(0.05)-sqr(i.y-0.565-a.y)<=0) {nq.push_back(i);break;}
               }
               if(a.y-0.390<=i.y&&i.y<a.y+0.565)
                  val=0.41375; // 中间的
               if(a.y-0.440<=i.y&&i.y<a.y-0.390) {
                  val=sqrt(sqr(0.05)-sqr(i.y+0.39-a.y))+0.36375; // 下半部分的圆角
                  if(sqr(0.05)-sqr(i.y+0.39-a.y)<=0) {nq.push_back(i);break;}
               }
               if(i.leftx<=a.x-val&&i.rightx>=a.x+val) { // 判定区间为原解区间的子区间，此时出现断点
                  nq.push_back({i.y,i.leftx,a.x-val ,i.speed,i.shiftdown,i.jump});
                  nq.push_back({i.y,a.x+val,i.rightx,i.speed,i.shiftdown,i.jump});
               } else { // 无断点
                  if(i.leftx >=a.x-val) i.leftx =fmax(i.leftx ,a.x+val);
                  if(i.rightx<=a.x+val) i.rightx=fmin(i.rightx,a.x-val);
                  if(i.leftx<=i.rightx)
                     nq.push_back(i);
               }
            }
         }
         val=nq;
      }
      return val;
   }
   vector<pkid> nxt() { // 返回当前 pkid 下一帧可能的位置,不会进行 fix, 因为要处理落地 bh.
      vector<pkid> result;
      {
         pkid t=*this;
         t.speed-=0.29;
         t.speed=fmax(-8.29,t.speed);
         t.y+=t.speed*0.02;
         result.push_back(t); // 原地不动
         t.leftx+=0.06;
         t.rightx+=0.06;
         result.push_back(t); // 右移一帧
         t.leftx-=0.12;
         t.rightx-=0.12;
         result.push_back(t); // 左移一帧
      }
      if(shiftdown) { // 此帧解除
         // TODO: 在使用仙人掌时此处不应当判断 shift down
         pkid t=*this;
         t.shiftdown=false;
         t.speed=t.speed*0.45;
         result.push_back(t); // 左移一帧
         // 解除时不刷新物理帧数。
      }
      if(jump) { // 此帧二段
         pkid t=*this;
         t.shiftdown=true;t.jump=false;
         t.speed=6;
         result.push_back(t);
         // 二段时不刷新物理帧数。
      }
      return result;
   }
   bool operator<(const pkid& other) const {
      if(y!=other.y) return y<other.y;
      if(leftx!=other.leftx) return leftx<other.leftx;
      if(rightx!=other.rightx) return rightx<other.rightx;
      if(speed!=other.speed) return speed<other.speed;
      if(shiftdown!=other.shiftdown) return shiftdown;
      if(jump!=other.jump) return jump;
      return false;
   }
};
string random_sentence() {
   static bool ird=false;
   if(!ird){ird=true;srand(time(0));}
   vector<string> Q={
      "据说抽到一些句子的概率会比别的句子概率更高,是真的嘛?",
      "为什么没有UI界面?才,才不是因为我不会写!",
      "谁想出来的这个功能,是五猫吗?",
      "一份古代的棋谱,听说看懂的人可以领悟国际象棋最高的奥秘? 1. e4 e5 2. Ke2 Ke7 3. Ke1 Ke8 4. Ke2 Ke7 5. Ke1 Ke8   1/2 - 1/2",
      "这哪无解了,不是能蹭边么?",
      "这里原来有一份狂热蓝调的字符画,但是因为太大所以被删掉了.jpg",
      "我觉得这里可以推歌 https://www.youtube.com/watch?v=uwszKrMXskQ",
      "【落英】darcilos 2024/4/21 1:24:23 其实是玩了21小时原神.",
      "welcome to os...needle!",
      "在勇者辛美尔死后 998244354 年,考古学家在一个尘封的闪存盘中发现了一份 pdf,在艰难的恢复了它的内容后,发现居然是一篇被鸽到现在还没更新的量子刺学.pdf!",
      "数学期望上来讲,假设有 n 句一言,那么期望要随机 n·O(ln n) 句一言才可以收集所有的一言.这个有什么用?",
      "向,clf,开战!向,糯米,开战!向,埃克斯,开战!",
      "北落师门A是一颗年青的蓝色恒星,它的行星家族刚刚起步。Telltale 星座是一个清晰定义的旋转物质环,是绰号为“索伦之眼”尘埃盘的一部分。当岩石的体积越来越大,足以将松散的碎石拉成紧密的整体时,就形成了这个环。",
      "I don’t wanna waste my life to the game, I can’t explain how.",
      "这个东西是在 2024/04/25 开始做的,有兴趣的人可以算算纳豆鸽了多久.",
      "珂朵莉树为这份代码提高了一百倍的效率,而这远远超过大部分放置游戏单个升级的提升.",
      
   };
   return Q[rand()%Q.size()];
}
void input_help() {
   cout << "指令列表:" << endl;
   cout << "- EXIT 退出" << endl;
   cout << "- RANDOM 发生一句随机的话" << endl;
   cout << "- NEWMAP 创建一个新地图" << endl;
   cout << "- LOADMAP 加载地图" << endl;
   cout << "- SAVEMAP 保存地图" << endl;
   cout << "- TYPELIST 物体编号列表" << endl;
   cout << "- ADDOBJ 新建一个物体" << endl;
   cout << "- REMOVEOBJ 删除一个物体" << endl;
   cout << "- OBJLIST 展示物体列表" << endl;
   cout << "- START 开始计算" << endl;
   cout << "- LOG 更新日志" << endl;
}
void input_exit() {
   if(current.x!=0) {
      string input;
      cout << "是否保存当前地图?(y/n)" << endl;
      cin >> input;
      transform(input.begin(),input.end(),input.begin(),::toupper);
      if(input=="Y") {
         cout << "文件名为:";
         cin >> input;
         current.print(input);
      }
   }
   
}
void input_newmap() {
   input_exit();
   cout << "新建地图的长与宽(使用格子作为单位,一个格子为 1*1,越小计算越快),最好不要将起点和终点放在地上，否则可能因为浮点误差原因导致死亡" << endl;
   cin >> current.x >> current.y;
   if(current.x<=0||current.y<=0) {
      cout << "不用的脑子可以捐给医院." << endl;
      current.x=current.y=0;
   } else
      cout << "创建了一个长度为 " << current.x << ",高度为 " << current.y << " 的空地图" << endl;
}
void input_loadmap() {
   cout << "从文件加载(当前地图将被覆盖):";
   string input;
   cin >> input;
   current.scanf(input);
   cout << endl;
}
void input_savemap() {
   if(current.x==0) {
      cout << "当前地图未被创建" << endl;
      return;
   }
   cout << "保存到文件(如果存在对应文件会覆盖):";
   string input;
   cin >> input;
   current.print(input);
   cout << endl;
}
void input_typelist() {
   object::typelist();
}
void input_addobj() {
   if(current.x==0) {
      cout << "当前地图未被创建" << endl;
      return;
   }
   int type;
   float x,y;
   cout << "type = ";
   cin >> type;
   cout << "x_pos = ";
   cin >> x;
   cout << "y_pos = ";
   cin >> y;
   current.obj.push_back({type,x,y});
   cout << endl;
}
void input_removeobj() {
   if(current.x==0) {
      cout << "当前地图未被创建" << endl;
      return;
   }
   cout << "删除物体:\n#";
   int x;cin >> x;
   if(cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(),'\n');
      cout << "不用的脑子可以捐给医院." << endl;
      return;
   }
   if(x<=0||x>(int)current.obj.size())
      cout << "对应物体不存在" << endl;
   else {
      current.obj.erase(current.obj.begin()+x-1);
      cout << endl;
   }
}
void input_objlist() {
   if(current.x==0) {
      cout << "当前地图未被创建" << endl;
      return;
   }
   cout << "目前总共有 " << current.obj.size() << " 物体:" << endl;
   int num=1;
   for(object i:current.obj) {
      cout << "#" << num << ": type=" << i.type << ", x=" << i.x << ", y=" << i.y << endl;
      num++;
   }
}
void input_start() {
   string input;int cactus;
   long long nodecnt=0;
   function<pair<int,int>()> check=[&]()->pair<int,int> {
      int start=0,end=0;
      for(object i:current.obj) {
         if(i.type==6) start++;
         if(i.type==7) end++;
      }
      return {start,end};
   };
   if(current.x==0) {
      cout << "当前地图未被创建" << endl;
      return;
   }
   if(check()!=pair<int,int>{1,1}) {
      cout << "起点和终点的数量不正确(预期为 1)" << endl;
      cout << "目前为:" << check().first << ',' << check().second << endl;
      return;
   }
   cout << "- 解刺机假设任何的下落都在起跳之前,并且终点是很平稳的下落,否则会导致不被预期的解." << endl;
   cout << "单跳仙人掌次数限制为(这个功能还没做好,随便输多少都无所谓):";
   cin >> cactus;
   cout << "搜索完整解空间(y),或仅构造最速解(n)" << endl;
   cin >> input;
   cout << "== 计算时不会考虑从抽象平台上进行下落的情况。因为这方面理论还没有完善 ==" << endl;
   transform(input.begin(),input.end(),input.begin(),::toupper);
   object starter{},ender{};
   for(object i:current.obj) {
      if(i.type==6) starter=i;
      if(i.type==7) ender=i;
   }
   if(input=="Y") {
      cout << "暂不支持" << endl;
      // double lower=-0.0056,upper=0.005;
      
   } else {
      double lower=-0.0056,upper=0.005;
      int num=1;
      for(double step=0.5;step>=0.5;step/=2,++num) {
         int cnt=1;
         for(double p=step;p<1;p+=step,++cnt) {
            double ya=p*(upper-lower)+lower;
            vector<pair<pkid,int>> Q;
            int left=0;
            set<pkid> rep;
            for(pkid i:pkid{starter.y-0.03+ya,starter.x-0.32,starter.x+0.32,7.2,true,true}.fix()) {
               Q.push_back({i,-1});
            }
            if(Q.empty()) continue;
            while(left!=(int)Q.size()) {
               pkid t=Q[left].first;
               left++;
               nodecnt++;
               if(rep.count(t)) continue;
               rep.insert(t);
               for(pkid i:t.nxt()) {
                  if(t.y>=ender.y-0.025&&i.y<=ender.y-0.0025) {
                     double L=ender.x-0.41375,R=ender.x+0.41375;
                     if(i.leftx<=L&&i.rightx>=L) {
                        pkid q=i;
                        q.rightx=L;
                        for(pkid j:q.fix())
                           Q.push_back({j,left-1});
                     }
                     if(i.leftx<=R&&i.rightx>=R) {
                        pkid q=i;
                        q.leftx=R;
                        for(pkid j:q.fix())
                           Q.push_back({j,left-1});
                     }
                     if(fmax(L,i.leftx)<=fmin(R,i.rightx)) {  // bh & 终点判断
                        if(i.y<ender.y-0.03875) i.y=ender.y-0.03;
                        if(!(i.fix().empty())) {
                           left--;
                           vector<pair<int,pkid>> res;
                           while(left!=-1) {
                              res.push_back({left,Q[left].first});
                              left=Q[left].second;
                           }
                           reverse(res.begin(),res.end());
                           cout << "ya/bh = " << ya << endl;
                           cout << "共计搜索了 " << nodecnt << " 个节点" << endl;
                           cout << "找到长度为 " << res.size() << " 的解:" << endl;
                           cout << "ID\t\tLEFTX\t\tRIGHTX\t\tYPOS\t\tSPEED\t\tJUMP" << endl;
                           for(auto r:res) 
                              cout << r.first << "\t\t" << r.second.leftx << "\t\t" << r.second.rightx << "\t\t" << r.second.y << "\t\t" << r.second.speed << "\t\t" << r.second.jump << endl;
                           return;
                        }
                     } else {
                        for(pkid j:i.fix())
                           Q.push_back({j,left-1});
                     }
                  } else {
                     for(pkid j:i.fix())
                        Q.push_back({j,left-1});
                  }
               }
            }
            cout << num << "层, 进度：" << cnt*step*100 << "% 累计节点: " << nodecnt << "                             \r";
         }
      }
      cout << "在 " << nodecnt << " 个节点之后没有发现解" << endl;
   }
   cout << endl;
}
void input_log() {
   cout << "2024-04-25 开始制作" << endl;
   cout << "2024-05-08 修复了对负数开平方导致 nan 的问题" << endl;
   cout << "2024-05-16 使用珂朵莉树以减少搜索节点开销" << endl;
}
void checking(string t) {
   if(t=="HELP") input_help(); else
   if(t=="EXIT") {input_exit();exit(0);} else
   if(t=="RANDOM") cout << random_sentence() << endl; else
   if(t=="NEWMAP") input_newmap(); else 
   if(t=="LOADMAP") input_loadmap(); else
   if(t=="SAVEMAP") input_savemap(); else
   if(t=="TYPELIST") input_typelist(); else
   if(t=="ADDOBJ") input_addobj(); else
   if(t=="REMOVEOBJ") input_removeobj(); else
   if(t=="OBJLIST") input_objlist(); else
   if(t=="START") input_start(); else
   if(t=="LOG") input_log(); else
   cout << "\'" << t << "\' 并不是一个可用的内部指令" << endl;
}
signed main() {
   SetConsoleOutputCP(CP_UTF8);
   cout << "Needle Solver [版本 : α]" << endl;
   cout << endl;
   while(1) {
      string input;
      cout << "> ";
      cin >> input;
      transform(input.begin(),input.end(),input.begin(),::toupper);
      checking(input);
      while(getchar()!='\n');
   }
}
