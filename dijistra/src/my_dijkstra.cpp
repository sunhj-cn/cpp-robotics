
#include <iostream>
#include <cmath>
#include <limits>
#include <queue>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <cstdio> // for remove
#include "../include/win_dir.h"
using namespace std;

class Node
{
public:
    int x;
    int y;
    float cost;
    bool known; // 表明节点是否已访问
    Node* P_node;
    bool has_obs;
    Node(int x_, int y_, float cost_, Node *P_node_ = NULL, bool known_ = false, bool has_obs_ = false) : x(x_), y(y_), cost(cost_), P_node(P_node_), known(known_), has_obs(has_obs_){};
};

// 仿函数
class cmp{
public:
    bool operator()(Node *n1, Node *n2)
    {
        return n1->cost > n2->cost; 
    }
};

vector<Node> get_motion_model()
{
    return {Node(1, 0, 1),
            Node(0, 1, 1),
            Node(-1, 0, 1),
            Node(0, -1, 1),
            Node(-1, -1, std::sqrt(2)),
            Node(-1, 1, std::sqrt(2)),
            Node(1, -1, std::sqrt(2)),
            Node(1, 1, std::sqrt(2))};
}

bool verify_node(Node const *node, int map_size)
{
    std::cout << "new node'x is : " << node->x << std::endl;

    if (node->x <= (map_size - 1) && node->x >= 0 && node->y <= (map_size - 1) && node->y >= 0)
    {
        return true;
    }
    return false;
}

bool CheckInQueue(Node *node, priority_queue<Node *, vector<Node *>, cmp> pq)
{
    while (!pq.empty())
    {
        Node *top = pq.top();
        pq.pop();
        if (top == node)
        {
            return true;
        }
    }
    return false;
}

void cacl_path(Node *node_G, cv::Mat &img, float img_reso){
    Node *node = node_G;
    while(node->P_node != NULL)
    {
        std::cout << "(" << node->x << ", " << node->y << ")" << std::endl;
        node = node->P_node;
        cv::rectangle(img,
                      cv::Point(node->x * img_reso + 1, node->y * img_reso + 1),
                      cv::Point((node->x + 1) * img_reso, (node->y + 1) * img_reso),
                      cv::Scalar(255, 0, 0), -1);
        cv::imshow("sunhj's dijkstra", img);
        cv::waitKey(1);
    }
}

bool dijkstra_planning(Node *node_S, Node *node_G, std::vector<std::vector<Node *>> &map, int map_size, int img_reso, std::vector<cv::Mat> &frames)
{
    // 初始化Q队列
    priority_queue<Node *, vector<Node *>, cmp> pq;
    pq.push(node_S);
    vector<Node> motion_model = get_motion_model();

    // 绘图初始化
    cv::Mat bg(img_reso * map_size,
               img_reso * map_size,
               CV_8UC3,
               cv::Scalar(255, 255, 255));
    // 绘制起始点
    cv::rectangle(bg,
                  cv::Point(node_S->x * img_reso + 1, node_S->y * img_reso + 1),
                  cv::Point((node_S->x + 1) * img_reso, (node_S->y + 1) * img_reso),
                  cv::Scalar(255, 0, 0), -1);

    // 绘制目标点
    cv::rectangle(bg,
                  cv::Point(node_G->x * img_reso + 1, node_G->y * img_reso + 1),
                  cv::Point((node_G->x + 1) * img_reso, (node_G->y + 1) * img_reso),
                  cv::Scalar(0, 0, 255), -1);

    // 绘制障碍物区域
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[0].size(); j++)
        {
            if (map[i][j]->has_obs == true)
            {
                cv::rectangle(bg,
                              cv::Point(map[i][j]->x * img_reso + 1, map[i][j]->y * img_reso + 1),
                              cv::Point((map[i][j]->x + 1) * img_reso, (map[i][j]->y + 1) * img_reso),
                              cv::Scalar(130, 130, 130), -1);
                cv::waitKey(1);
            }
        }
    }

    cv::Mat frame;
    bg.copyTo(frame); // 保证所有帧的尺寸一致
    frames.push_back(frame);

    //  循环
    while (!pq.empty())
    {
        Node *v = pq.top();
        pq.pop();
        if (v->known == 1){continue;}
        else{v->known = true;}

        // 判断是否是终点
        if (v == node_G)
        {
            cacl_path(node_G, bg, img_reso);
            cv::Mat frame;
            bg.copyTo(frame); // 保证所有帧的尺寸一致
            frames.push_back(frame);
            return true;
        }

        // 对该节点进行应用所有动作
        for (int i = 0; i < motion_model.size(); i++)
        {
            Node *new_node = new Node(v->x + motion_model[i].x,
                                      v->y + motion_model[i].y,
                                      v->cost + motion_model[i].cost,
                                      v);

            if (!verify_node(new_node, map_size)) // 如果不在墙内
            {
                delete new_node;
                continue;
            }
            else // 如果在墙内，继续判断是否已访问
            {
                if (map[new_node->x][new_node->y]->known || map[new_node->x][new_node->y]->has_obs) // 是否已访问或不是障碍物
                {
                    delete new_node;
                    continue;
                }
            }

            // 更新节点参数
            if (new_node->cost < map[new_node->x][new_node->y]->cost)
            {
                map[new_node->x][new_node->y]->cost = new_node->cost;
                map[new_node->x][new_node->y]->P_node = v;
                // std::cout << "find a new v" << std::endl;
            }
            if (!CheckInQueue(map[new_node->x][new_node->y], pq))
            {
                pq.push(map[new_node->x][new_node->y]);
            }

            cv::rectangle(bg,
                          cv::Point(new_node->x * img_reso + 1, new_node->y * img_reso + 1),
                          cv::Point((new_node->x + 1) * img_reso, (new_node->y + 1) * img_reso),
                          cv::Scalar(0, 255, 0));
            
            cv::Mat frame;
            bg.copyTo(frame); // 保证所有帧的尺寸一致
            frames.push_back(frame);
   
            cv::imshow("sunhj's dijkstra", bg);
            cv::waitKey(1);
            delete new_node;
        }
    }
    return false;
}

int main()
{
    int map_size = 100;
    float xs = 30;
    float ys = 35;
    float xg = 30;
    float yg = 30;
    float reso = 1;
    int img_reso = 5;

    // 初始化所有节点map
    std::vector<Node *> row(map_size, new Node(0, 0, 10000, NULL));
    std::vector<std::vector<Node *>> map(map_size, row);

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[0].size(); j++)
        {
            map[i][j] = new Node(i, j, 1e5);
            if ((map[i][j]->x >= int(map.size() / 3)) && (map[i][j]->x <= int(map.size() / 3) + 5) && (map[i][j]->y >= int(map.size() / 4)) && (map[i][j]->y <= int(map.size() / 2)))
            {
                map[i][j]->has_obs = true;
            }
        }
    }
    // 获取map中起始点和终点
    Node *node_S = map[(int)std::round(xs / reso)][(int)std::round(ys / reso)];
    Node *node_G = map[(int)std::round(xg / reso)][(int)std::round(yg / reso)];
    node_S->cost = 0; // start point's cost will be set to 0.
    std::vector<cv::Mat> frames; // 用于存储gif

    bool SUCCESS = dijkstra_planning(node_S, node_G, map, map_size, img_reso, frames);
    if (SUCCESS)
    {
        std::cout << "SUCCESS" << std::endl;
        int delay = 10;
        string folderPath = ".//numbered_images//";
        if (CreateDirectoryIfNotExists(folderPath)) {
            for (int i = 0; i < frames.size(); ++i) {
                cv::imwrite(folderPath  + std::to_string(i) + std::string(".png"), frames[i]);
            }
        }
    }
    std::cout << "Press any key to quit... " << std::endl;
    std::cin.get();
    return 0;
}