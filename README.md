# C++ Software Deployment Automation with Workflow Reporting

**Academic Project**

- **Degree / Program:** B.Tech in Artificial Intelligence & Robotics / Computer Science & Engineering
- **Subject:** Software Systems for Robotics and AI
- **Author / Student:** Sathvik
- **Project Type:** Course-Based Project

---

## 1. Project Overview

C++ Software Deployment Automation with Workflow Reporting is a CI/CD automation project that demonstrates how software compilation, testing, packaging, execution, and reporting can be automated using GitHub Actions.

The project uses a simple C++ calculator application as its sample software.

Whenever a developer pushes code changes to the GitHub repository, GitHub Actions automatically starts a workflow that:

1. Retrieves the latest source code.
2. Configures and compiles the C++ application using CMake.
3. Executes automated tests using CTest.
4. Builds a Docker image containing the application.
5. Runs the application inside a Docker container.
6. Generates JSON and Markdown workflow reports.
7. Uploads the reports and available execution logs as downloadable artifacts.

The main objective is to demonstrate how a single Git push can trigger an automated software delivery workflow.

## 2. Problem Statement

Software development involves several repetitive activities, including compiling source code, running tests, packaging applications, and verifying execution.

Performing these tasks manually after every code modification can consume time and may lead to inconsistent processes.

This project addresses that problem by implementing an automated CI/CD pipeline using GitHub Actions.

The pipeline automatically performs the required software development and packaging tasks whenever code is pushed to the repository, while also maintaining a report of the workflow execution.

## 3. Project Objectives

- Automate C++ application compilation.
- Execute automated test cases after compilation.
- Integrate Docker for application packaging.
- Execute and verify the packaged application.
- Generate workflow execution reports.
- Upload reports and logs as GitHub Actions artifacts.
- Demonstrate failure handling when compilation, testing, or packaging fails.
- Reduce repetitive manual development and deployment activities.

## 4. Technology Stack

| Technology | Purpose |
|---|---|
| C++17 | Sample calculator application |
| Git | Version control |
| GitHub | Remote source code repository |
| GitHub Actions | CI/CD workflow automation |
| CMake | C++ project configuration and build management |
| GCC / Clang | C++ compilation |
| CTest | Automated test execution |
| Docker | Application packaging and container execution |
| Python 3 | Workflow report generation |
| JSON | Machine-readable workflow report |
| YAML | GitHub Actions workflow configuration |
| Markdown | Documentation and human-readable report |

## 5. System Architecture

The project follows an automated software delivery workflow.

**Workflow:**

1. The developer modifies the C++ source files.
2. Changes are committed and pushed to GitHub.
3. GitHub detects the push event.
4. GitHub Actions starts a workflow on a GitHub-hosted Ubuntu runner.
5. CMake configures and compiles the application.
6. CTest executes the automated test suite.
7. Docker builds an image containing the compiled application.
8. The Docker container runs the calculator application.
9. Python generates workflow execution reports.
10. GitHub Actions uploads the reports and available logs as artifacts.

## 6. Project Folder Structure

```text
cpp-deployment-automation/
│
├── src/
│   ├── main.cpp
│   ├── calculator.cpp
│   └── calculator.h
│
├── tests/
│   └── test_calculator.cpp
│
├── scripts/
│   └── generate_report.py
│
├── .github/
│   └── workflows/
│       └── ci-cd.yml
│
├── CMakeLists.txt
├── Dockerfile
├── .gitignore
└── README.md
```

### Description of important files

- `main.cpp`: Entry point of the calculator application.
- `calculator.cpp`: Contains the arithmetic operation implementations.
- `calculator.h`: Declares the calculator functions.
- `test_calculator.cpp`: Contains automated test cases.
- `CMakeLists.txt`: Configures the application build and CTest.
- `Dockerfile`: Defines how the application is built and packaged into a Docker image.
- `ci-cd.yml`: Defines the automated GitHub Actions workflow.
- `generate_report.py`: Generates JSON and Markdown workflow reports.
- `README.md`: Provides project documentation and instructions.

## 7. Calculator Application

The project uses a simple C++ calculator as its sample application.

It supports:

- Addition
- Subtraction
- Multiplication
- Division

The calculator also handles division-by-zero errors using exception handling.

The application supports command-line arguments.

Examples:

```bash
./build/calculator 15 + 7
./build/calculator 20 '*' 4
./build/calculator 100 / 5
```

Expected output for the first command:

```text
[RESULT] 15 + 7 = 22.0000
```

When the application is executed without arguments, it runs a deterministic demonstration containing sample arithmetic operations and division-by-zero verification. This mode is used for automated Docker execution.

## 8. How Git and GitHub Are Used

Git is used to track modifications to the project files.

GitHub hosts the remote repository and receives committed changes.

The developer uses the following commands to upload changes:

```bash
git add .
git commit -m "Updated calculator application"
git push
```

The push event triggers the configured GitHub Actions workflow.

## 9. How GitHub Actions Works

GitHub Actions is used to automate the CI/CD process.

The workflow configuration is stored in:

```text
.github/workflows/ci-cd.yml
```

The workflow is configured to trigger on:

- Pushes to the `main` branch.
- Pull requests targeting the `main` branch.

For pull requests, the workflow performs compilation and automated testing.

For pushes to `main`, the complete workflow also performs Docker packaging, container execution, reporting, and artifact upload.

The workflow runs on a temporary GitHub-hosted Ubuntu environment.

## 10. Compilation and Build Process

CMake manages the C++ build configuration.

The project uses the C++17 standard.

The build process creates two executable targets:

- `calculator`: Main calculator application.
- `test_calculator`: Automated test executable.

The compilation process is performed automatically by GitHub Actions after a push.

## 11. Automated Testing

CTest is used to execute the automated calculator test suite.

The test suite checks:

- Addition.
- Subtraction.
- Multiplication.
- Division.
- Negative number calculations.
- Division-by-zero handling.
- Invalid operation handling.

The test executable calls the actual calculator functions.

If the tests fail, the dependent Docker packaging and container execution stages are skipped.

## 12. Docker Packaging and Execution

Docker is used to package the compiled calculator application into a Docker image.

The project uses a multi-stage Dockerfile.

The first stage:

- Installs the required compiler and CMake tools.
- Copies the C++ source code.
- Configures and compiles the application.
- Runs CTest as an additional build-time check.

The second stage:

- Uses a smaller runtime environment.
- Copies the compiled calculator executable.
- Runs the application using a non-privileged user.

The Docker image is built automatically by GitHub Actions.

The container is then executed to verify that the packaged application works correctly.

Example local Docker commands:

```bash
docker build -t cpp-calculator:latest .
docker run --rm cpp-calculator:latest
```

The container runs the calculator's deterministic demonstration mode when no arguments are provided.

This project demonstrates temporary container execution on the GitHub Actions runner. It does not deploy the application to a permanent cloud server.

## 13. Workflow Report Generation

Python is used to generate workflow execution reports.

The report generator is located at:

```text
scripts/generate_report.py
```

It produces two reports:

1. `workflow-report.json`
2. `workflow-report.md`

The reports include information such as:

- Project name.
- Repository name.
- Branch name.
- Commit ID.
- Workflow run ID and number.
- Trigger event.
- Execution timestamp.
- Compilation status.
- Testing status.
- Docker build status.
- Container execution status.
- Overall workflow status.

The workflow passes execution results and GitHub metadata to the Python script through environment variables.

The reports provide machine-readable and human-readable summaries of the workflow execution.

## 14. Artifact Upload

GitHub Actions uploads generated reports and available execution logs as a downloadable artifact.

The artifact name is:

```text
robot-workflow-report
```

The artifact can contain:

- `workflow-report.json`
- `workflow-report.md`
- `test-results.txt`
- `build-logs.txt`
- `docker-build-logs.txt`
- `container-output.txt`

Files are uploaded when they exist.

To download the artifact:

1. Open the GitHub repository.
2. Select the Actions tab.
3. Open the required workflow run.
4. Scroll to the Artifacts section.
5. Download `robot-workflow-report`.

Workflow logs show the live output of individual steps, whereas artifacts preserve generated files for later inspection.

## 15. Failure Handling

The pipeline handles failures at different stages.

### Compilation failure

- Compilation is marked as failed.
- Testing is skipped.
- Docker packaging is skipped.
- Container execution is skipped.
- A failure report is generated.
- Artifact upload is attempted.

### Testing failure

- Compilation succeeds.
- Testing fails.
- Docker packaging is skipped.
- Container execution is skipped.
- A failure report is generated.
- Artifact upload is attempted.

### Docker build failure

- Compilation succeeds.
- Testing succeeds.
- Docker packaging fails.
- Container execution is skipped.
- A failure report is generated.
- Artifact upload is attempted.

### Container execution failure

- Compilation succeeds.
- Testing succeeds.
- Docker image creation succeeds.
- Container execution fails.
- A failure report is generated.
- Artifact upload is attempted.

### Successful execution

When all required stages succeed, the workflow completes successfully and uploads the generated reports and logs.

## 16. Installation Requirements

For local development on macOS, install the following tools.

### Install Xcode Command Line Tools

```bash
xcode-select --install
```

This provides the Apple Clang compiler and related development tools.

### Install Homebrew packages

Install Git and CMake if they are not already available:

```bash
brew install git cmake
```

Docker Desktop is optional for local Docker testing. It is not required for Docker operations performed on GitHub-hosted runners.

### Verify installations

```bash
git --version
cmake --version
clang++ --version
python3 --version
```

If Docker is installed and running, verify it using:

```bash
docker --version
docker info
```

## 17. Building the Project Locally

Open Terminal and navigate to the project directory:

```bash
cd cpp-deployment-automation
```

Configure the project:

```bash
cmake -B build -S .
```

Compile the application:

```bash
cmake --build build
```

Run the calculator demonstration:

```bash
./build/calculator
```

Run an individual calculation:

```bash
./build/calculator 25 + 15
```

## 18. Running Automated Tests Locally

After building the project, execute:

```bash
ctest --test-dir build --output-on-failure
```

This runs the registered CTest test suite and displays the test results.

The test executable can also be run directly:

```bash
./build/test_calculator
```

## 19. Creating and Connecting the GitHub Repository

Create a new GitHub repository named:

```text
cpp-deployment-automation
```

Do not initialize the remote repository with an additional README or other files if you are pushing the existing project as the initial commit.

From the project directory, initialize Git if it has not already been initialized:

```bash
git init
```

Add the project files:

```bash
git add .
```

Create the initial commit:

```bash
git commit -m "Initial project implementation"
```

Set the main branch:

```bash
git branch -M main
```

Connect the local project to the GitHub repository:

```bash
git remote add origin https://github.com/YOUR_USERNAME/cpp-deployment-automation.git
```

Replace `YOUR_USERNAME` with your actual GitHub username.

Push the project:

```bash
git push -u origin main
```

## 20. Observing the Automated Workflow

After pushing the project:

1. Open the GitHub repository.
2. Select the Actions tab.
3. Open the automatically triggered workflow.
4. Select the latest workflow run.
5. Inspect the individual stages.

The workflow displays the progress of:

- Source code checkout.
- Environment setup.
- C++ compilation.
- Automated testing.
- Docker packaging.
- Container execution.
- Workflow report generation.
- Artifact upload.

This allows the developer to observe the automation happening in the background.

## 21. Demonstration Plan

The following sequence can be used for the academic project demonstration