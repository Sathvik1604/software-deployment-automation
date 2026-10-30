
#!/usr/bin/env python3

"""
Workflow Report Generator

Generates JSON and Markdown reports using actual
GitHub Actions step outcomes and workflow metadata.
"""

import json
import os
import sys
from datetime import datetime, timezone


def normalize_status(raw_status):
    """Convert GitHub Actions outcomes into standard statuses."""

    if not raw_status:
        return "SKIPPED"

    status = raw_status.strip().lower()

    if status == "success":
        return "SUCCESS"

    if status in ("failure", "timed_out", "cancelled"):
        return "FAILURE"

    if status == "skipped":
        return "SKIPPED"

    return "UNKNOWN"


def determine_overall_status(stages, is_pull_request):
    """Determine the overall pipeline result."""

    required_stages = [
        "checkout",
        "environment_setup",
        "compilation",
        "automated_testing"
    ]

    if not is_pull_request:
        required_stages.extend([
            "docker_build",
            "container_execution"
        ])

    for stage in required_stages:
        if stages.get(stage) != "SUCCESS":
            return "FAILURE"

    return "SUCCESS"


def read_file_preview(file_path, max_lines=15):
    """Read a short preview of a generated log file."""

    if not os.path.exists(file_path):
        return "File not generated or stage skipped."

    try:
        with open(
            file_path, "r", encoding="utf-8", errors="replace"
        ) as file:
            lines = []

            for _ in range(max_lines):
                line = file.readline()
                if not line:
                    break
                lines.append(line)

            content = "".join(lines).strip()

            return content if content else "(Empty file)"

    except OSError as error:
        return f"Could not read file: {error}"


def generate_report():

    # Collect workflow metadata
    project_name = os.getenv(
        "PROJECT_NAME",
        "C++ Software Deployment Automation with Workflow Reporting"
    )

    repository = os.getenv(
        "GITHUB_REPOSITORY", "local/cpp-deployment-automation"
    )

    branch = os.getenv("GITHUB_REF_NAME", "local")

    commit_sha = os.getenv("GITHUB_SHA", "local-dev-commit")

    commit_message = os.getenv(
        "GIT_COMMIT_MESSAGE", "Local execution"
    )

    run_id = os.getenv("GITHUB_RUN_ID", "local-run")

    run_number = os.getenv("GITHUB_RUN_NUMBER", "0")

    event_name = os.getenv("GITHUB_EVENT_NAME", "local")

    output_dir = os.getenv("REPORT_OUTPUT_DIR", ".")

    is_pull_request = event_name == "pull_request"

    # Read actual GitHub Actions step outcomes
    stages = {
        "checkout": normalize_status(
            os.getenv("STEP_CHECKOUT_STATUS")
        ),

        "environment_setup": normalize_status(
            os.getenv("STEP_SETUP_STATUS")
        ),

        "compilation": normalize_status(
            os.getenv("STEP_COMPILE_STATUS")
        ),

        "automated_testing": normalize_status(
            os.getenv("STEP_TEST_STATUS")
        ),

        "docker_build": normalize_status(
            os.getenv("STEP_DOCKER_BUILD_STATUS")
        ),

        "container_execution": normalize_status(
            os.getenv("STEP_CONTAINER_EXEC_STATUS")
        )
    }

    # Docker stages are intentionally skipped for pull requests.
    if is_pull_request:
        stages["docker_build"] = "SKIPPED"
        stages["container_execution"] = "SKIPPED"

    # Enforce dependent-stage skipping.
    if stages["compilation"] != "SUCCESS":
        stages["automated_testing"] = "SKIPPED"
        stages["docker_build"] = "SKIPPED"
        stages["container_execution"] = "SKIPPED"

    elif stages["automated_testing"] != "SUCCESS":
        stages["docker_build"] = "SKIPPED"
        stages["container_execution"] = "SKIPPED"

    elif stages["docker_build"] != "SUCCESS":
        if not is_pull_request:
            stages["container_execution"] = "SKIPPED"

    overall_status = determine_overall_status(
        stages, is_pull_request
    )

    timestamp = datetime.now(timezone.utc).strftime(
        "%Y-%m-%d %H:%M:%S UTC"
    )

    # Prepare output directory
    os.makedirs(output_dir, exist_ok=True)

    json_path = os.path.join(
        output_dir, "workflow-report.json"
    )

    markdown_path = os.path.join(
        output_dir, "workflow-report.md"
    )

    # Construct JSON report
    report_data = {
        "project": {
            "name": project_name,
            "category": "Academic B.Tech CI/CD Automation Project",
            "subject": "Software Systems for Robotics and AI"
        },

        "metadata": {
            "repository": repository,
            "branch": branch,
            "commit_id": commit_sha[:8],
            "commit_id_full": commit_sha,
            "commit_message": commit_message,
            "run_id": str(run_id),
            "run_number": str(run_number),
            "trigger_event": event_name,
            "timestamp": timestamp
        },

        "stages": stages,

        "overall_status": overall_status,

        "is_pull_request": is_pull_request
    }

    # Write JSON report
    with open(json_path, "w", encoding="utf-8") as file:
        json.dump(report_data, file, indent=4)

    # Build Markdown report
    stage_descriptions = {
        "checkout": "Retrieved repository source code.",
        "environment_setup": "Checked compiler and required tools.",
        "compilation": "Configured and compiled the C++ application.",
        "automated_testing": "Executed the CTest test suite.",
        "docker_build": "Built the Docker application image.",
        "container_execution": "Executed and verified the Docker container."
    }

    markdown = [
        f"# {project_name}",
        "",
        "## Automated Workflow Execution Report",
        "",
        f"**Generated:** {timestamp}",
        f"**Overall Status:** {overall_status}",
        "",
        "## Execution Metadata",
        "",
        f"- Repository: `{repository}`",
        f"- Branch: `{branch}`",
        f"- Commit: `{commit_sha[:8]}`",
        f"- Commit message: {commit_message}",
        f"- Workflow Run ID: `{run_id}`",
        f"- Workflow Run Number: `{run_number}`",
        f"- Trigger: `{event_name}`",
        "",
        "## Pipeline Stage Results",
        "",
        "| Stage | Status | Description |",
        "|---|---|---|"
    ]

    for stage, status in stages.items():
        markdown.append(
            f"| {stage.replace('_', ' ').title()} "
            f"| **{status}** "
            f"| {stage_descriptions[stage]} |"
        )

    markdown.extend([
        "",
        "## Test Output",
        "",
        "```text",
        read_file_preview(
            os.path.join(output_dir, "test-results.txt")
        ),
        "```",
        "",
        "## Build Output",
        "",
        "```text",
        read_file_preview(
            os.path.join(output_dir, "build-logs.txt")
        ),
        "```",
        "",
        "## Docker Build Output",
        "",
        "```text",
        read_file_preview(
            os.path.join(output_dir, "docker-build-logs.txt")
        ),
        "```",
        "",
        "## Container Output",
        "",
        "```text",
        read_file_preview(
            os.path.join(output_dir, "container-output.txt")
        ),
        "```",
        "",
        "## Final Result",
        ""
    ])

    if overall_status == "SUCCESS":
        markdown.append(
            "All required pipeline stages completed successfully."
        )
    else:
        markdown.append(
            "One or more required stages failed or did not complete. "
            "Inspect the stage statuses and workflow logs."
        )

    markdown.extend([
        "",
        "---",
        "",
        "*Generated automatically by scripts/generate_report.py.*",
        "*Artifact upload status is available in the GitHub Actions run.*"
    ])

    # Write Markdown report
    with open(
        markdown_path, "w", encoding="utf-8"
    ) as file:
        file.write("\n".join(markdown))

    print(f"[REPORT] JSON report created: {json_path}")
    print(f"[REPORT] Markdown report created: {markdown_path}")
    print(f"[REPORT] Overall status: {overall_status}")

    # Report generation succeeded even if an earlier pipeline stage failed.
    # GitHub Actions separately preserves the original job failure.
    return 0


if __name__ == "__main__":
    sys.exit(generate_report())
