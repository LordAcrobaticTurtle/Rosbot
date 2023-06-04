// Need to check how much you trust measurement vs prediction
// You do that by comparing covariance matrix of sensor vs covariance matrix of prediction

class KalmanFilter {
    public:
        KalmanFilter();

        void setup();
        /**
         * @brief Apply prediction step
         * Predict next state
         * Predicted estimate covariance
         */
        void prediction();

        /**
         * @brief apply update step
         * 
         */
        void update();

    private:

        unsigned int tn1;
        unsigned int tn2;

        // Covariance matrix
        double covariance[3][3];

};