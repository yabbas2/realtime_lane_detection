#include "eliminatefalsedetection.h"

vector<vector<int>> eliminateFalseDetection::getLines()
{return lines;}
/*
template <typename RandomIt, typename KeyFunc>
void sort_by_key_2(RandomIt first, RandomIt last, KeyFunc func)
{
    using KeyT = decltype(func(*first));
    std::vector<std::pair<KeyT, RandomIt> > ordering;
    ordering.reserve(last - first);

    for (; first != last; ++first) {
        ordering.emplace_back(func(*first), first);
    }

    // now sort this vector by the ordering - we're going
    // to sort ordering, but each swap has to do iter_swap too
    quicksort_with_benefits(ordering, 0, ordering.size());
}

template <typename Key, typename Iter>
void quicksort_with_benefits(std::vector<std::pair<Key,Iter>>& A, size_t p, size_t q) {
    if (p < q) {
        size_t r = partition_with_benefits(A, p, q);
        quicksort_with_benefits(A, p, r);
        quicksort_with_benefits(A, r+1, q);
    }
}

template <typename Key, typename Iter>
size_t partition_with_benefits(std::vector<std::pair<Key,Iter>>& A, size_t p, size_t q) {
    auto key = A[p].first;
    size_t i = p;
    for (size_t j = p+1; j < q; ++j) {
        if (A[j].first < key) {
            ++i;
            std::swap(A[i].first, A[j].first);
            std::iter_swap(A[i].second, A[j].second);
        }
    }

    if (i != p) {
        std::swap(A[i].first, A[p].first);
        std::iter_swap(A[i].second, A[p].second);
    }
    return i;
}
*/
void eliminateFalseDetection::falseElimination(Mat hsv_frame, Mat ipm_frame, vector<vector<float>> &l)
{
    // eliminate False lines function
    vector<vector<int>> tmp_lines;
    vector<vector<float>>::iterator it;
    for(it = l.begin(); it != l.end(); ++it)
    {
        float x1 = it[0][0], y1 = it[0][1], x2 = it[0][2], y2 = it[0][3];
        if(y1 > y2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }
        float theta = fastAtan2((y2 - y1), (x2 - x1));
        if (theta > 180)
            theta -= 180;
        float length = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
        if ((threshold_angle_min < theta) && (theta < threshold_angle_max) && (length >= threshold_length))
            tmp_lines.push_back({(int)x1, (int)y1, (int)x2, (int)y2, (int)theta, (int)length, 0});
    }
    // remove shadows function
    Mat mask;
    vector<float> contours;
    vector<int> boundary_min = {0, 100, 0};
    vector<int> boundary_max = {154, 255, 154};
    inRange(hsv_frame, boundary_min, boundary_max, mask);
    bitwise_and(ipm_frame, ipm_frame, ipm_frame, mask=mask);
    findContours(mask, contours, 1, 2);
    drawContours(hsv_frame, contours, 0, (0, 255, 255), 2);
    vector<vector<int>>::iterator it_lines;
    vector<float>::iterator it_contours;
    for(it_lines = tmp_lines.begin(); it_lines != tmp_lines.end(); ++it_lines)
    {
        int flag = 0;
        if(contours.empty())
        {
            lines = tmp_lines;
            return;
        }
        for(it_contours = contours.begin(); it_contours != contours.end(); ++it_contours)
        {
            Point2f a = ((float)tmp_lines[0][0], (float)tmp_lines[0][1]);
            Point2f b = ((float)tmp_lines[0][2], (float)tmp_lines[0][3]);
            double dist1 = pointPolygonTest(contours, a, true);
            double dist2 = pointPolygonTest(contours, b, true);
            if(dist1 >= -17 || dist2 >= -17)
                flag = 1;
        }
        if(flag == 1)
            continue;
        else
            lines.push_back({(int)tmp_lines[0][0], (int)tmp_lines[0][1], (int)tmp_lines[0][2], (int)tmp_lines[0][3], (int)tmp_lines[0][4], (int)tmp_lines[0][5], (int)tmp_lines[0][6]});
    }
}
