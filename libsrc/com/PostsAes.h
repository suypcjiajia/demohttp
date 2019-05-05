#ifndef POSTSAES_H
#define POSTSAES_H

#include <string>
using namespace std;

class PostsAes
{
public:
    PostsAes();
    ~PostsAes();
    //POST
    string HttpsPost(string psUrl, string psPostData);
};

#endif // POSTSAES_H
