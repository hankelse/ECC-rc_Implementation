
# one line with the number of nodes
# for each node, one line containing the node's ID (integer) and its degree
# finally, the list of edges (pair of IDs separated by space, one on each line in any order)

# datasets = [
# "datasets/ca-AstroPh.txt", 
# "datasets/ca-CondMat.txt", 
# "datasets/ca-GrQc.txt", 
# "datasets/ca-HepPh.txt", 
# "datasets/ca-HepTh.txt", 
# "datasets/cit-HepTh.txt",
# "datasets/cit-HepPh.txt",
# "datasets/email-Enron.txt",
# "datasets/email-EuAll.txt",
# "datasets/p2p-Gnutella31.txt",
# "datasets/soc-Slashdot0811.txt",
# "datasets/soc-Slashdot0902.txt",
# "datasets/wiki-Vote.txt"]
datasets = [
"new_snap_datasets/amazon0302.txt",
"new_snap_datasets/amazon0312.txt",
"new_snap_datasets/roadNet-CA.txt",
"new_snap_datasets/roadNet-PA.txt",
"new_snap_datasets/roadNet-TX.txt",
"new_snap_datasets/web-BerkStan.txt",
"new_snap_datasets/web-Google.txt",
]

OUTPUT_DIR = "new_nde_datasets/"
INPUT_DIR = datasets[0].split("/")[0]+ "/"

filenames = [ds.split("/")[1] for ds in datasets]
print(filenames)


# FILENAME = "clique.txt"


# OUTPUT_PATH = ""

NUM_COMMENT_LINES = 4



def get_file_lines(filename):
    """Read and return all lines from the specified file."""
    try:
        with open(filename, 'r') as file:
            lines = file.readlines()
        return lines
    
    except FileNotFoundError:
        print ("Couldn't open file")
        return []

def write_lines_to_file(lines, filename):
    """Write a list of lines to the specified file."""
    try:
        with open(filename, 'w') as file:
            for line in lines:
                file.write(line + '\n')  # Adds a newline after each line
        print(f"Successfully written {len(lines)} lines to '{filename}'.")
    except Exception as e:
        print(f"An error occurred: {e}")


def convert(filename):

    SNAP_FILE_PATH = INPUT_DIR + filename
    NDE_FILE_PATH = OUTPUT_DIR + filename

    nodes = []
    node_degrees = {}

    edges = [] 
    node_deg_pairs = []

    #GET LINES in [ [node1, node2], [node1, node2], ... ] form
    raw_lines = get_file_lines(SNAP_FILE_PATH)
    clean_lines = []
    for raw_line in raw_lines[NUM_COMMENT_LINES::]:
        line = raw_line.split()
        try:
            line = [int(line[0]), int(line[1])]
        except:
            print("DIDN'T WORK", line)
        clean_lines.append(line)

    #Get nodes[], degrees{}, edges[] for list of edges
    for edge_list in clean_lines:
        edges.append(  str(edge_list[0]) + "\t" + str(edge_list[1])   )

        for node in edge_list:
            if node in node_degrees:
                node_degrees[node] += 1
            else:
                node_degrees[node] = 1
                nodes.append(node)

    #get node_deg_pairs
    for node in nodes:
        node_deg_pairs.append(str(node) + "\t" + str(node_degrees[node]))

    print("NODES:", nodes)
    print("DEGREES:", node_degrees, "\n")

    print("EDGES:", edges)
    print("NODE DEG:", node_deg_pairs)

    output_lines = [str(len(nodes))] + node_deg_pairs + edges
    write_lines_to_file(output_lines, NDE_FILE_PATH)
 


def main():
    for fn in filenames:
        convert(fn)

main()
        














