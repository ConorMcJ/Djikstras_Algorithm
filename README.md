# README: UIC Open Street Map Navigation Application

## Project Overview

This project is a **C++ application** designed for navigating the **University of Illinois at Chicago (UIC)** campus using data from an **OpenStreetMap (OSM)** XML file. The application parses geographic data, including nodes (map points), footways (walkable paths), and buildings, and constructs a graph for further interaction, such as route planning and building location.

The project was developed for **CS 251** at UIC, Fall 2023, under the guidance of **Adam T. Koehler, PhD**. It is based on an original project variation by **Joe Hummel, PhD**.

### Key Technologies and References:
- **C++**: Core programming language used for the project.
- **TinyXML**: Library used for parsing XML-based OSM files. [GitHub Repository](https://github.com/leethomason/tinyxml2)
- **OpenStreetMap (OSM)**: The source of geographic data. [OpenStreetMap](https://www.openstreetmap.org)
- **OSM Documentation**:
  - [Main OSM Wiki](https://wiki.openstreetmap.org/wiki/Main_Page)
  - [Map Features](https://wiki.openstreetmap.org/wiki/Map_Features)
  - [Node Definition](https://wiki.openstreetmap.org/wiki/Node)
  - [Way Definition](https://wiki.openstreetmap.org/wiki/Way)
  - [Relation Definition](https://wiki.openstreetmap.org/wiki/Relation)

## Features

- **OSM Data Loading and Parsing**: The application loads an XML-based OSM file and extracts key data related to nodes, footways, and buildings.
- **Graph Construction**: Builds a graph from map data where:
  - **Vertices** represent geographic nodes.
  - **Edges** represent connections (footways) between nodes.
- **Statistics Display**: After parsing, the program displays useful statistics such as the number of nodes, footways, buildings, vertices, and edges.
- **Campus Navigation**: Provides further interaction with UIC's map data through the `application()` function, allowing users to explore the map and find buildings or routes.
  
## How the Application Works

1. **OSM File Loading**: 
   - The program attempts to load an OSM file (default: `uic-2024.osm`). You can specify a different filename if needed by uncommenting the input prompt in the code.

2. **Parsing OSM Data**:
   - **Nodes**: Represents latitude and longitude coordinates for map points.
   - **Footways**: Represents walking paths on the campus, parsed based on the `footway` tag.
   - **Buildings**: Information about university buildings, including name and approximate location.

3. **Graph Construction**:
   - The program creates a graph where each vertex corresponds to a geographic node, and each edge represents a footway that connects two nodes.

4. **Program Statistics**:
   - After parsing, the program outputs the number of nodes, footways, buildings, vertices, and edges.

5. **Application Interaction**:
   - The `application()` function provides additional functionality for users to interact with the graph and map data, such as finding buildings or identifying potential navigation routes.

6. **Completion**:
   - After all operations are complete, the program prints "** Done **" and exits.

## How to Use the Application

1. **Dependencies**:
   - Install **TinyXML2** for XML parsing: [TinyXML2 GitHub](https://github.com/leethomason/tinyxml2).
   
2. **Compilation**:
   - Use the following command to compile the program with its dependencies:
     ```bash
     g++ -o uic_navigation main.cpp osm.cpp graph.cpp application.cpp tinyxml2.cpp
     ```

3. **Running the Application**:
   - To run the program:
     ```bash
     ./uic_navigation
     ```

   - The default file `uic-2024.osm` will be used unless you specify a different filename.

4. **Providing a Custom OSM File**:
   - Uncomment the input prompt in `main()` to provide a different OSM file:
     ```cpp
     // cout << "Enter map filename> ";
     // getline(cin, filename);
     ```

5. **User Interaction**:
   - Once the map is loaded and parsed, the `application()` function will allow interaction with the campus map data.

## File Descriptions

- **osm.cpp**: Contains functions for loading and parsing the OSM data, including nodes, footways, and buildings.
- **graph.h**: Defines the graph data structure used to represent the map.
- **application.h**: Contains the main application logic for user interaction.
- **tinyxml2.h**: External XML parser library for handling OSM files.

## OSM Data Parsing Details

- **Nodes**: Parsed from `<node>` elements in the XML. They are stored in a map with their IDs as keys and coordinates (latitude and longitude) as values.
- **Footways**: Parsed from `<way>` elements with a `highway=footway` tag. Node IDs associated with each footway are stored in a `FootwayInfo` object.
- **Buildings**: Parsed from `<way>` elements with a `building=university` tag. The program computes an average latitude and longitude to represent the building's location based on its perimeter nodes.

## Known Issues and Future Enhancements

- **Door Locations**: The OSM data does not provide explicit door locations for buildings, so building locations are approximated using the centroid of their perimeter nodes.
- **Future Features**:
  - Implement pathfinding algorithms like Dijkstra’s or A* to provide shortest path navigation between buildings.
  - Enable real-time interactive commands for navigation.
  - Improve UI and provide better error handling for invalid or missing map data.

## License

This project was developed as part of a course at **University of Illinois at Chicago** and is intended for educational purposes.

---

**Developed By**:  
- **Adam T Koehler, PhD**, University of Illinois Chicago, CS 251, Fall 2023  
- **Original Variation By**: Joe Hummel, PhD, University of Illinois at Chicago
