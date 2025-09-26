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
                    cp gtestresults.xml $WORKSPACE/gtestresults.xml
                '''
            }
        }
        stage('Coverage report') {
            steps {
                sh '''
                    cd $git_checkout_root/middlewaresw
                    bash ./run_coverage.sh
                    zip -r $WORKSPACE/coverage_html.zip coverage_html
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
            xunit (
                thresholds: [ skipped(), failed(failureThreshold: '0') ],
                tools: [ GoogleTest(pattern: 'gtestresults.xml') ]
            )
        }
    }
}
