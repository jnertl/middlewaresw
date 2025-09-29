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
                    CURRENT_DIR=$PWD
                    cd $git_checkout_root/middlewaresw
                    ./build.sh
                    zip -r -j $CURRENT_DIR/middlewaresw.zip build_application/middlewaresw
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
                    CURRENT_DIR=$PWD
                    cd $git_checkout_root/middlewaresw
                    bash ./run_coverage.sh
                    zip -r $CURRENT_DIR/coverage_html.zip coverage_html
                '''
            }
        }
    }
    post {
        always {
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
