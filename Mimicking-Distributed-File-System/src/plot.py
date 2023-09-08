# plot the graph between number of lines received and time taken
import matplotlib.pyplot as plt
import csv
import sys

def plot_graph(data_file):
    x = []
    y = []
    # assert that the file exists
    try:
        with open(data_file, 'r') as csvfile:
            plots = csv.reader(csvfile, delimiter=',')
            # skip the header
            next(plots)
            for row in plots:
                x.append(int(row[0]))
                y.append(float(row[1]))
        plt.plot(x,y, label='Time taken')
        plt.xlabel('Number of lines received')
        plt.ylabel('Time taken')
        plt.title('Graph between number of lines received and time taken')
        plt.legend()
        # save the plot
        plt.savefig('../plots/latency.png')
        # plt.show()
    except Exception as e:
        print(e)

if __name__ == "__main__":
    # python3 plot.py <data_file>
    # get the file in the parent directory
    # data_file = "../logs/plot_data.csv"
    # give absolute path using os
    # data_file = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'logs', 'plot_data.csv'))

    print("Plotting graph between number of lines received and time taken")
    print("Reading data from file: ", sys.argv[1], flush=True)
    plot_graph(sys.argv[1])
