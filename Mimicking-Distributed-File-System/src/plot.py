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
    if len(sys.argv) != 2:
        print("Usage: python3 plot.py <data_file>")
        exit(1)
    print("Plotting graph between number of lines received and time taken")
    print("Reading data from file: ", sys.argv[1], flush=True)
    plot_graph(sys.argv[1])
