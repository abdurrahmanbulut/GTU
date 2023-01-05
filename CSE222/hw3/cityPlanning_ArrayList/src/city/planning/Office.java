package city.planning;

public class Office extends Building{
    private String jobType;

    public Office(String jobType, String owner) {
        this.jobType = jobType;
        this.owner = owner;
    }
    public Office(){
    }

    public String getJobType() {
        return jobType;
    }

    public void setJobType(String jobType) {
        this.jobType = jobType;
    }

    public void focus(){
        System.out.println("Job type: " + jobType);
    }
}
