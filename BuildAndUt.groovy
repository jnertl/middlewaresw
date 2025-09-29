pipeline {
    agent any
    environment {
        git_checkout_root = '/var/jenkins_home/workspace/build_and_ut_git_checkout'
    }
    stages {
        stage('Checkout') {
            steps {
                sh '''
                    echo "Git checkout root:${git_checkout_root}"
                    rm -fr "${git_checkout_root}" || true
                    mkdir -p "${git_checkout_root}"
                    cd "${git_checkout_root}"
                    git clone --single-branch --branch main https://github.com/jnertl/middlewaresw.git
                    echo "middlewaresw"
                    git --no-pager -C middlewaresw/ show --summary
                '''
            }
        }
        stage('Cleanup workspace') {
            steps {
                sh '''
                    rm -fr "${WORKSPACE}/gtestresults.xml" || true
                    rm -fr "${WORKSPACE}/coverage_html" || true
                    rm -fr "${WORKSPACE}/failure_analysis.txt" || true
                    rm -fr "${WORKSPACE}/middlewaresw.zip" || true
                    rm -fr "${WORKSPACE}/coverage_html.zip" || true
                '''
            }
        }
        stage('Build binaries') {
            steps {
                sh '''
                    cd "${git_checkout_root}/middlewaresw"
                    ./build.sh
                '''
            }
        }
        stage('Run unit tests') {
            steps {
                sh '''
                    cd "${git_checkout_root}/middlewaresw"
                    bash ./run_tests.sh
                '''
            }
        }
        stage('Coverage report') {
            steps {
                sh '''
                    cd "${git_checkout_root}/middlewaresw"
                    bash ./run_coverage.sh
                '''
            }
        }
    }
    post {
        always {
            sh '''
                zip -r -j "${WORKSPACE}/middlewaresw.zip" "$git_checkout_root/middlewaresw/build_application/middlewaresw" || true
                cp "${git_checkout_root}/middlewaresw/gtestresults.xml" "${WORKSPACE}/" || true
                cp -r "${git_checkout_root}/middlewaresw/coverage_html" "${WORKSPACE}/" || true
                zip -r -j "${WORKSPACE}/coverage_html.zip" "${WORKSPACE}/coverage_html" || true
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
            xunit(
                thresholds: [ skipped(), failed(failureThreshold: '0') ],
                tools: [ GoogleTest(pattern: 'gtestresults.xml') ]
            )
        }
        success {
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
