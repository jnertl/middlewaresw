pipeline {
    agent any
    environment {
        git_checkout_root = '/var/jenkins_home/workspace/build_and_ut_git_checkout'
    }
    stages {
        stage('Checkout') {
            steps {
                sh '''
                    echo $PWD
                    rm -fr $git_checkout_root || true
                    mkdir -p $git_checkout_root
                    cd $git_checkout_root
                    git clone --single-branch --branch main https://github.com/jnertl/middlewaresw.git
                    echo "middlewaresw"
                    git --no-pager -C middlewaresw/ show --summary
                '''
            }
        }
        stage('Cleanup workspace') {
            steps {
                sh '''
                    rm -fr gtestresults.xml || true
                    rm -fr coverage_html || true
                    rm -fr failure_analysis.txt || true
                    rm -fr middlewaresw.zip || true
                    rm -fr coverage_html.zip || true
                '''
            }
        }
        stage('Build binaries') {
            steps {
                sh '''
                    cd $git_checkout_root/middlewaresw
                    ./build.sh
                    zip -r -j $WORKSPACE/middlewaresw.zip build_application/middlewaresw
                '''
            }
        }
        stage('Run unit tests') {
            steps {
                sh '''
                    cd $git_checkout_root/middlewaresw
                    bash ./run_tests.sh
                '''
            }
        }
        stage('Coverage report') {
            steps {
                sh '''
                    cd $git_checkout_root/middlewaresw
                    bash ./run_coverage.sh
                    zip -r $WORKSPACE/coverage_html.zip coverage_html
                    cp -r coverage_html $WORKSPACE/coverage_html
                '''
            }
        }
    }
    post {
        always {
            sh '''
                echo 'Build failed. Executing failure handler...'
                rm -fr mcpdemo || true
                git clone --single-branch --branch main https://github.com/jnertl/mcpdemo.git
                cd mcpdemo
                git --no-pager show --summary
                export SOURCE_DIR="$git_checkout_root/middlewaresw"
                export REQUIREMENTS_FILE="$SOURCE_DIR/feature_requirements.md"
                export CONTEXT_FILE="$SOURCE_DIR/src_context.txt"
                ./create_context.sh
                export GTEST_JOB_LOG="$(cat $git_checkout_root/middlewaresw/gtestresults.xml)"

                ./ongoing_printer.sh \
                /usr/local/bin/mcphost \
                --temperature 0.1 --top-p 0.8 --top-k 50 --max-tokens 4096 \
                --quiet --stream=false \
                --system-prompt ./system_prompts/jenkins_results_assistant.txt \
                script user_prompts/analyse_failed_jenkins_job.sh \
                >&1 | tee failure_analysis.txt
            '''

            archiveArtifacts(
                artifacts: 'middlewaresw.zip',
                fingerprint: true,
                allowEmptyArchive: true
            )
            archiveArtifacts(
                artifacts: 'gtestresults.xml',
                fingerprint: true,
                allowEmptyArchive: true
            )
            archiveArtifacts(
                artifacts: 'coverage_html.zip',
                fingerprint: true,
                allowEmptyArchive: true
            )
            archiveArtifacts(
                artifacts: 'failure_analysis.txt',
                fingerprint: true,
                allowEmptyArchive: true
            )
        }
        success {
            xunit(
                thresholds: [ skipped(), failed(failureThreshold: '0') ],
                tools: [ GoogleTest(pattern: 'gtestresults.xml') ]
            )
            publishHTML(target: [
                reportName: 'Coverage Report',
                reportDir: 'coverage_html',
                reportFiles: 'index.html',
                keepAll: true,
                alwaysLinkToLastBuild: true,
                allowMissing: false
            ])
        }
    }
}
